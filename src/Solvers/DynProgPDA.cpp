#include "DynProgPDA.hpp"

#include <iostream>
#include <algorithm>
#include <iterator>
#include <iomanip>
#include <fstream>

namespace Alg
{
    DynProgPDA::DynProgPDA(const String &iFilename, int T, int K)
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
        
        HashMap<int, HashMap<String, int>> Q_t_c;
        HashMap<int, HashMap<int, double>> f_t_x;
        HashMap<int, std::set<int>> Q_t_c_sets;
        for (const auto& [t, _]: flight.q_t_k) {
            Q_t_c_sets[t] = std::set<int>();
            for (const auto& [k, c_k] : flight.c_k) {
                int potentialClients = q_t_c(t, c_k);
                Q_t_c[t][k] = potentialClients;
                Q_t_c_sets[t].insert(potentialClients);
                f_t_x[t][potentialClients] = c_k;
            }
        }

        HashMap<int, HashMap<int, double>> S_t_q;
        S_t_q.reserve(flight.q_t_k.size());
        auto calculate_max = [&](const double max, const int t, const int q, const int x) {
            if (t == 1) {
                return std::max(max, f_t_x[t][x]*x);
            }
            return std::max(max, f_t_x[t][x]*x + S_t_q[t-1][q-x]);
        };

        // TODO: сделать итерацию по t нормально
        for (int t = 1; t <= flight.q_t_k.size(); ++t) {
            std::cout << "t = " << t << std::endl;
            for (int q = 0; q <= flight.Q; ++q) {
                double max = 0;
                for (const auto& x: Q_t_c_sets.at(t)) {
                    if (x > q) {
                        break;
                    }
                    if (x == 0) {
                        continue;
                    }
                    max = calculate_max(max, t, q, x);
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

    std::vector<String> DynProgPDA::ExtractSolution(const Data::FlightData &flight, HashMap<int, HashMap<int, double>> &S_t_q) const
    {
        return std::vector<String>();
    }

    void DynProgPDA::Solve()
    {
        for (const auto& flight : _df.flights) {
            _results.push_back(SolveSingleFlight(flight));
        }
    }

    void DynProgPDA::ToCsv(const String& iFileName) const
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


