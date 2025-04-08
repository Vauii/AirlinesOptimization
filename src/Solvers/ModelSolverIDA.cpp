#include "ModelSolverIDA.hpp"

#include <iostream>
#include <fstream>
#include <iomanip>
#include <memory>

#include "DynProgPDA.hpp"

namespace Alg
{
    ModelSolverIDA::ModelSolverIDA(const String& iFilename, const String& iSolverType,
         bool iUseRelaxation, bool iUseDynProgResults)
        : _df(iFilename),
          _solverType(iSolverType),
          _useRelaxation(iUseRelaxation), 
          _useDynProgResults(iUseDynProgResults)
    {
        _results.reserve(_df.Size());
    }

    double ModelSolverIDA::SolveSingleFlight(const Data::FlightData& iFlight, HashMap<int, HashMap<String,
         double>>& iSolution, bool iUseRelaxation, bool iUseDynProgResults, String iSolverType)
    {   
        Vector<Pair<String, double>> c_k_sorted(iFlight.c_k.begin(),iFlight.c_k.end());
        std::sort(c_k_sorted.begin(), c_k_sorted.end(), [](const auto& a, const auto& b) {
            return a.second < b.second;
        });

        HashMap<int, Alg::String> c_t;
        c_t.reserve(iFlight.q_t_k.size());
        if (iUseDynProgResults) {
            c_t = DynProgPDA::SolveSingleFlight(iFlight);
        }
        else {
            for (const auto& [t, _]: iFlight.q_t_k) {
                for (const auto& [k, c_k] : c_k_sorted) {
                    if (iFlight.q_t_k.at(t).at(k) > 0) {
                        c_t[t] = k;
                        break;
                    }
                }
            }
        }
        
        HashMap<int, Vector<Pair<String, double>>> c_t_k_sorted;
        for (const auto& [t, _]: c_t) {
            for (const auto& [k, c_k] : c_k_sorted) {
                if (iFlight.c_k.at(c_t.at(t)) <= c_k && iFlight.q_t_k.at(t).at(k) > 0) {
                    c_t_k_sorted[t].emplace_back(k, c_k);
                }
            }
        }

        auto q_t_c = [&](int t, double c) {
            int potentialClients = 0;
            for (const auto& [k, c_k] : c_t_k_sorted.at(t)) {
                if (c_k >= c) {
                    potentialClients += iFlight.q_t_k.at(t).at(k);
                }
            }
            return potentialClients;
        };
        HashMap<int, HashMap<String, int>> Q_t_ck;
        for (const auto& [t, _]: c_t) {
            for (const auto& [k, c_k] : c_t_k_sorted.at(t)) {
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
        HashMap<int, HashMap<String, OR::MPVariable*>> x_t_k;
        for (const auto& [t, _]: c_t) {
            for (const auto& [k, c_k] : c_t_k_sorted[t]) {
                if (iUseRelaxation) {
                    x_t_k[t][k] = pSolver->MakeNumVar(0.0, Q_t_ck[t][k], "x_" + std::to_string(t) + "_" + k);
                }
                else
                    x_t_k[t][k] = pSolver->MakeIntVar(0.0, Q_t_ck[t][k], "x_" + std::to_string(t) + "_" + k);
            }
        }

        // constraints
        // 1. sum_t sum_k x_t_k <= Q
        OR::MPConstraint* constraint1 = pSolver->MakeRowConstraint(0, iFlight.Q);
        for (const auto& [t, _]: c_t) {
            for (const auto& [k, c_k] : c_t_k_sorted[t]) {
                constraint1->SetCoefficient(x_t_k[t][k], 1);
            }
        }

        // constraints
        // 2. x_t_k + (sum_(j >= k) sum_(k_t <= i < k) x_t_i*q_t_j/Q_t_i)  <= Q_t_k for all k >= k^t, t
        HashMap<int, HashMap<String, OR::MPConstraint*>> constraint2; 
        for (const auto& [t, _]: c_t) {
            for (int k = 0; k < c_t_k_sorted[t].size(); ++k) {
                const auto classK = c_t_k_sorted[t][k].first;
                constraint2[t][classK] = pSolver->MakeRowConstraint(0, Q_t_ck[t][classK]);
                constraint2[t][classK]->SetCoefficient(x_t_k[t][classK], 1);
                // if (t == 300) {
                //     std::cout << "\n";
                // }
                for (int j = k; j < c_t_k_sorted[t].size(); ++j) 
                {
                    for (int i = 0; i <= j; ++i) {
                        if (i == k) {
                            continue;
                        }
                        const auto classJ = c_t_k_sorted[t][j].first;
                        const auto classI = c_t_k_sorted[t][i].first;
                        assert(Q_t_ck[t][classI] > 0);
                        constraint2[t][classK]->SetCoefficient(x_t_k[t][classI], iFlight.q_t_k.at(t).at(classJ) * 1.0 / Q_t_ck[t][classI]);   
                    }
                }
                
            }
            // if (t == 300) {
            //     std::cout << "\n";
            // }
            // for (int k = 0; k < c_t_k_sorted[t].size(); ++k) {
            //     const auto classK = c_t_k_sorted[t][k].first;
            //     std::cout << "constr_" << t << "_" << classK << "   ";
            //     for (int j = 0; j < c_t_k_sorted[t].size(); ++j) {
            //         const auto classJ = c_t_k_sorted[t][j].first;
            //         std::cout << x_t_k[t][classJ]->name() << "=" << constraint2[t][classK]->GetCoefficient(x_t_k[t][classJ]) << "   ";
            //     }
            //     std::cout << "\n";
            // }
        }

        // objective
        OR::MPObjective* pObjective = pSolver->MutableObjective();
        for (const auto& [t, _]: c_t) {
            for (const auto& [k, c_k] : c_t_k_sorted[t]) {
                pObjective->SetCoefficient(x_t_k[t][k], c_k);
            }
        }
        pObjective->SetMaximization();

        LOG(INFO) << "Solving with " << pSolver->SolverVersion();
        const OR::MPSolver::ResultStatus resultStatus = pSolver->Solve();
        if (resultStatus != OR::MPSolver::OPTIMAL) {
            LOG(FATAL) << "The problem does not have an optimal solution.";
        }
        LOG(INFO) << "Optimal objective value = " << pObjective->Value();
        double ticketsSold = 0;
        Vector<Pair<int, String>> sorted_c_t(c_t.begin(), c_t.end());
        std::sort(sorted_c_t.begin(), sorted_c_t.end(), [](const auto& a, const auto& b) {
            return a.first < b.first;
        });
        for (const auto& [t, _]: sorted_c_t) {
            for (const auto& [k, c_k] : c_t_k_sorted[t]) {
                iSolution[t][k] = x_t_k[t][k]->solution_value();
                ticketsSold += iSolution[t][k];
                // if (iSolution[t][k] > 0)
                //     std::cout << t << " " << k << " Value: " << iSolution[t][k] << "\n"; 
            }
        }
        const double EPS = 10e-6;
        assert(ticketsSold <= iFlight.Q);
        return pObjective->Value();
    }

    double ModelSolverIDA::SolveFlight(const Data::FlightData& iFlight, HashMap<int, HashMap<String, double>>& iSolution) const
    {   
        return SolveSingleFlight(iFlight, iSolution, _useRelaxation, _useDynProgResults, _solverType);
    }

    void ModelSolverIDA::Solve()
    {
        for (const auto& flight : _df.flights) {
            auto solution = HashMap<int, HashMap<String, double>>();
            const auto revenue = SolveFlight(flight, solution);
            _results.emplace_back(revenue, solution);
        }
    }

    void ModelSolverIDA::ToCsv(const String& iFileName) const
    {   
        std::ofstream outputFile(iFileName);

        if (!outputFile.is_open()) {
            std::cerr << "Failed to open the file!\n";
            return;
        }

        outputFile << "FLTDATE,ORIG,DEST,Score\n";
        for (size_t i = 0; i < _results.size(); ++i) {
            outputFile << std::fixed << _df.flights[i].date << ","
                << _df.flights[i].origin << ","
                << _df.flights[i].destination << ","
                << std::setprecision(0) << _results[i].first << ",";
            for (const auto& [t, x_t]: _results[i].second) {
                for (const auto& [k, x_t_k] : x_t) {
                    if (x_t_k > 0)
                        outputFile << t << " " << k << " x: " << std::setprecision(2) << x_t_k << ","; 
                }
            }
            outputFile << "\n";

        }
        outputFile.close();
        std::cout << "Results saved to " << iFileName << std::endl;
    }
}
