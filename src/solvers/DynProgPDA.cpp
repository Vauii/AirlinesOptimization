#include "DynProgPDA.hpp"

#include <iostream>
#include <algorithm>
#include <iterator>
#include <iomanip>
#include <fstream>

namespace Alg
{
    DynProgPDA::DynProgPDA(const std::string &iFilename, int T, int K)
        : _df(iFilename, T, K)
    {
        N = _df.Size() / (T * K);
        _results.reserve(N);
    }

    double DynProgPDA::SolveSingleFlight(const Data::FlightData& flight) const
    {
        auto q_t_c = [&](int t, int c) {
            int potentialClients = 0;
            for (const auto& [k, c_k] : flight.c_k) {
                if (c_k >= c) {
                    potentialClients += flight.q_t_k.at(t).at(k);
                }
            }
            return potentialClients;
        };   
        
        std::unordered_map<int, std::unordered_map<std::string, int>> Q_t_ck;
        for (const auto& [t, _]: flight.q_t_k) {
            for (const auto& [k, c_k] : flight.c_k) {
                Q_t_ck[t][k] = q_t_c(t, c_k);
            }
        }
        std::unordered_map<int, std::vector<int>> Q_t_ck_sorted_asc;
        for (const auto& [t, q_t_ck] : Q_t_ck) {
            Q_t_ck_sorted_asc[t] = std::vector<int>();
            for (const auto& [k, q_t_k] : q_t_ck) {
                Q_t_ck_sorted_asc[t].push_back(q_t_k);
            }
            std::sort(Q_t_ck_sorted_asc[t].begin(), Q_t_ck_sorted_asc[t].end(), std::less<int>());
        }

        std::vector<std::pair<std::string, double>> c_k_sorted(flight.c_k.begin(), flight.c_k.end());
        std::sort(c_k_sorted.begin(), c_k_sorted.end(), [](const auto& a, const auto& b) {
            return a.second > b.second;
        });

        auto f_t_x = [&](int t, int x) {
            for (const auto& [k, c_k] : c_k_sorted) {
                if (x - flight.q_t_k.at(t).at(k) == 0) {
                    return flight.c_k.at(k);
                }
                x -= flight.q_t_k.at(t).at(k);
            }
            return 0.0;
        };

        std::unordered_map<int, std::unordered_map<int, double>> S_t_q;
        S_t_q.reserve(flight.q_t_k.size());
        // TODO: сделать итерацию по t нормально
        for (const auto& q : Q_t_ck_sorted_asc.at(1)) {
            double max = 0;
            for (const auto& x: Q_t_ck_sorted_asc.at(1)) {
                if (x > q) {
                    break;
                }
                max = std::max(max, f_t_x(1, x)*x);
            }
            S_t_q[1][q] = max;
        }

        for (int t = 2; t <= flight.q_t_k.size(); ++t) {
            for (const auto& q : Q_t_ck_sorted_asc.at(t)) {
                double max = 0;
                for (const auto& x: Q_t_ck_sorted_asc.at(t)) {
                    if (x > q) {
                        break;
                    }
                    max = std::max(max, f_t_x(t, x)*x + S_t_q[t-1][q-x]);
                }
                S_t_q[t][q] = max;
            }
        }
        
        double maxRevenue = 0;
        for (const auto& [q, revenue] : S_t_q.at(flight.q_t_k.size())) {
            maxRevenue = std::max(maxRevenue, revenue);
        }
        return maxRevenue;
    }

    void DynProgPDA::Solve()
    {
        for (const auto& flight : _df.flights) {
            _results.push_back(SolveSingleFlight(flight));
        }
    }

    void DynProgPDA::ToCsv(const std::string& iFileName) const
    {   
        std::ofstream outputFile(iFileName);

        if (!outputFile.is_open()) {
            std::cerr << "Failed to open the file!\n";
            return;
        }

        outputFile << "FLTDATE,ORIG,DEST,Score\n";
        for (size_t i = 0; i < _results.size(); ++i) {
            // TODO: fix precision    
            outputFile << std::fixed << std::setprecision(0) << _df.flights[i].date << ","
                << _df.flights[i].origin << ","
                << _df.flights[i].destination << ","
                << _results[i] << "\n";
        }
        outputFile.close();

        std::cout << "Results saved to " << iFileName << std::endl;
    }
}


