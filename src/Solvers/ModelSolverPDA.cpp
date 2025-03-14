#include "ModelSolverPDA.hpp"

#include <iostream>
#include <fstream>
#include <iomanip>
#include <memory>

namespace Alg
{
    ModelSolverPDA::ModelSolverPDA(const String& iFilename, int T, int K, const String& iSolverType)
        : _df(iFilename, T, K)
    {
        N = _df.Size() / (T * K);
        _results.reserve(N);
        _solverType = iSolverType;
    }

    double ModelSolverPDA::SolveSingleFlight(const Data::FlightData& flight, Vector<Pair<double, int>>& iSolution) const
    {
        auto q_t_c = [&](int t, double c) {
            int potentialClients = 0;
            for (const auto& [k, c_k] : flight.c_k) {
                if (c_k >= c) {
                    potentialClients += flight.q_t_k.at(t).at(k);
                }
            }
            return potentialClients;
        };
        HashMap<int, HashMap<String, int>> Q_t_ck;
        for (const auto& [t, _]: flight.q_t_k) {
            for (const auto& [k, c_k] : flight.c_k) {
                Q_t_ck[t][k] = q_t_c(t, c_k);
            }
        } 
        
        // solver
        std::unique_ptr<OR::MPSolver> pSolver(OR::MPSolver::CreateSolver(_solverType));
        if (!pSolver) {
            LOG(WARNING) << "Could not create solver " << _solverType;
            return 0.0;
        }

        // variables
        HashMap<int, HashMap<String, OR::MPVariable*>> c_t_k;
        for (const auto& [t, _]: flight.q_t_k) {
            for (const auto& [k, _] : flight.c_k) {
                c_t_k[t][k] = pSolver->MakeBoolVar("c_" + std::to_string(t) + "_" + k);
            }
        }

        // constraints
        // 1. sum_k c_t_k[t][k] == 1 for all t
        for (const auto& [t, _]: flight.q_t_k) {
            OR::MPConstraint* const constraint = pSolver->MakeRowConstraint(1, 1);
            for (const auto& [k, _] : flight.c_k) {
                constraint->SetCoefficient(c_t_k[t][k], 1);
            }
        }

        // 2. sum_t sum_k c_t_k[t][k]*Q_t_ck[t][k] <= Q
        OR::MPConstraint* const constraint = pSolver->MakeRowConstraint(0, flight.Q);
        for (const auto& [t, _]: flight.q_t_k) {
            for (const auto& [k, _] : flight.c_k) {
                constraint->SetCoefficient(c_t_k[t][k], Q_t_ck[t][k]);
            }
        }

        // objective
        OR::MPObjective* const pObjective = pSolver->MutableObjective();
        for (const auto& [t, _]: flight.q_t_k) {
            for (const auto& [k, c_k] : flight.c_k) {
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

        for (size_t t = _df.flights.back().q_t_k.size(); t >= 1; --t) {
            for (const auto& [k, _] : flight.c_k) {
                if (c_t_k[t][k]->solution_value() > 0.5) {
                    iSolution.emplace_back(flight.c_k.at(k), q_t_c(t, flight.c_k.at(k)));
                    break;
                }
            }
        }
        
        return pObjective->Value();
    }

    void ModelSolverPDA::Solve()
    {
        for (const auto& flight : _df.flights) {
            auto solution = Vector<Pair<double, int>>();
            const auto revenue = SolveSingleFlight(flight, solution);
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
            // TODO: fix precision    
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
