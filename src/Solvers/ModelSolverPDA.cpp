#include "ModelSolverPDA.hpp"

#include <iostream>
#include <fstream>
#include <iomanip>
#include <memory>

namespace Alg
{
    ModelSolverPDA::ModelSolverPDA(const String& iFilename, int T, int K, const String& iSolverType, bool iUseRelaxation)
        : _df(iFilename, T, K)
    {
        _useRelaxation = iUseRelaxation;
        N = _df.Size() / (T * K);
        _results.reserve(N);
        _solverType = iSolverType;
    }

    double ModelSolverPDA::SolveSingleFlight(const Data::FlightData& iFlight,
        Vector<Pair<double, int>>& iSolution, bool iUseRelaxation, String iSolverType)
    {
        auto q_t_c = [&](int t, double c) {
            int potentialClients = 0;
            for (const auto& [k, c_k] : iFlight.c_k) {
                if (c_k >= c) {
                    potentialClients += iFlight.q_t_k.at(t).at(k);
                }
            }
            return potentialClients;
        };
        HashMap<int, HashMap<String, int>> Q_t_ck;
        for (const auto& [t, _]: iFlight.q_t_k) {
            for (const auto& [k, c_k] : iFlight.c_k) {
                Q_t_ck[t][k] = q_t_c(t, c_k);
            }
        } 
        
        // solver
        std::unique_ptr<OR::MPSolver> pSolver(OR::MPSolver::CreateSolver(iSolverType));
        if (!pSolver) {
            LOG(WARNING) << "Could not create solver " << iSolverType;
            return 0.0;
        }
        if (iSolverType == "SCIP") {
            pSolver->SetSolverSpecificParametersAsString("limits/gap=0.0");
        }
        else if (iSolverType == "CBC") {
            pSolver->SetSolverSpecificParametersAsString("ratio=0.0");
        }

        // variables
        HashMap<int, HashMap<String, OR::MPVariable*>> c_t_k;
        for (const auto& [t, _]: iFlight.q_t_k) {
            for (const auto& [k, _] : iFlight.c_k) {
                if (iUseRelaxation) {
                    c_t_k[t][k] = pSolver->MakeNumVar(0.0, 1.0, "c_" + std::to_string(t) + "_" + k);
                }
                else
                    c_t_k[t][k] = pSolver->MakeBoolVar("c_" + std::to_string(t) + "_" + k);
            }
        }

        // constraints
        // 1. sum_k c_t_k[t][k] == 1 for all t
        for (const auto& [t, _]: iFlight.q_t_k) {
            OR::MPConstraint* const constraint = pSolver->MakeRowConstraint(1, 1);
            for (const auto& [k, _] : iFlight.c_k) {
                constraint->SetCoefficient(c_t_k[t][k], 1);
            }
        }

        // 2. sum_t sum_k c_t_k[t][k]*Q_t_ck[t][k] <= Q
        OR::MPConstraint* const constraint = pSolver->MakeRowConstraint(0, iFlight.Q);
        for (const auto& [t, _]: iFlight.q_t_k) {
            for (const auto& [k, _] : iFlight.c_k) {
                constraint->SetCoefficient(c_t_k[t][k], Q_t_ck[t][k]);
            }
        }

        // objective
        OR::MPObjective* const pObjective = pSolver->MutableObjective();
        for (const auto& [t, _]: iFlight.q_t_k) {
            for (const auto& [k, c_k] : iFlight.c_k) {
                pObjective->SetCoefficient(c_t_k[t][k], c_k*Q_t_ck[t][k]);
            }
        }
        pObjective->SetMaximization();

        LOG(INFO) << "Solving with " << pSolver->SolverVersion();
        const OR::MPSolver::ResultStatus resultStatus = pSolver->Solve();
        if (resultStatus != OR::MPSolver::OPTIMAL) {
            LOG(FATAL) << "The problem does not have an optimal solution.";
        }
        LOG(INFO) << "Optimal objective value = " << pObjective->Value();

        int ticketsRemaining = iFlight.Q;
        for (size_t t = iFlight.q_t_k.size(); t >= 1; --t) {
            auto it = std::max_element(
                c_t_k[t].begin(),
                c_t_k[t].end(),
                [](const auto& a, const auto& b) {
                    return a.second->solution_value() < b.second->solution_value();
                }
            );
            const auto potentialClients = q_t_c(t, iFlight.c_k.at(it->first));
            iSolution.emplace_back(iFlight.c_k.at(it->first), potentialClients);
        }
        
        return pObjective->Value();
    }

    double ModelSolverPDA::SolveFlight(const Data::FlightData& iFlight, Vector<Pair<double, int>>& iSolution) const
    {   
        return SolveSingleFlight(iFlight, iSolution, _useRelaxation, _solverType);
    }

    void ModelSolverPDA::Solve()
    {
        for (const auto& flight : _df.flights) {
            auto solution = Vector<Pair<double, int>>();
            const auto revenue = SolveFlight(flight, solution);
            _results.emplace_back(revenue, solution);
        }
    }

    void ModelSolverPDA::ToCsv(const String& iFileName) const
    {   
        std::ofstream outputFile(iFileName);

        if (!outputFile.is_open()) {
            std::cerr << "Failed to open the file!\n";
            return;
        }

        outputFile << "FLTDATE,ORIG,DEST,Score";
        for (size_t t = _df.flights.back().q_t_k.size(); t >= 1; --t) {
            outputFile << "," << t << "c" << "," << t << "x";
        }
        outputFile << "\n";
        for (size_t i = 0; i < _results.size(); ++i) {
            outputFile << std::fixed << std::setprecision(0) << _df.flights[i].date << ","
                << _df.flights[i].origin << ","
                << _df.flights[i].destination << ","
                << _results[i].first;
            for (const auto& [price, clientsCount] : _results[i].second) {
                outputFile << "," << price << "," << clientsCount;
            }
            outputFile << "\n";

        }
        outputFile.close();

        std::cout << "Results saved to " << iFileName << std::endl;
    }
}
