#include "DynProgPDA.hpp"

#include <iostream>
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

    double DynProgPDA::SolveSingleFlight(const Data::FlightData& flight, Vector<Pair<double, int>>& iSolution) const
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
        
        HashMap<int, HashMap<int, Pair<String, double>>> f_t_x;
        HashMap<int, Set<int>> Q_t_c;
        for (const auto& [t, _]: flight.q_t_k) {
            Q_t_c[t] = Set<int>();
            for (const auto& [k, c_k] : flight.c_k) {
                int potentialClients = q_t_c(t, c_k);
                Q_t_c[t].insert(potentialClients);
                f_t_x[t][potentialClients].second = c_k;
                f_t_x[t][potentialClients].first = k;
            }
        }

        HashMap<int, HashMap<int, double>> S_t_q;
        S_t_q.reserve(flight.q_t_k.size());
        auto CalculateMax = [&](const double max, const int t, const int q, const int x) {
            if (t == 1) {
                return std::max(max, f_t_x[t][x].second*x);
            }
            return std::max(max, f_t_x[t][x].second*x + S_t_q[t-1][q-x]);
        };

        // TODO: сделать итерацию по t нормально
        for (int t = 1; t <= flight.q_t_k.size(); ++t) {
            std::cout << "t = " << t << std::endl;
            for (int q = 0; q <= flight.Q; ++q) {
                double max = 0;
                for (const auto& x: Q_t_c.at(t)) {
                    if (x > q) {
                        break;
                    }
                    if (x == 0) {
                        continue;
                    }
                    max = CalculateMax(max, t, q, x);
                }
                S_t_q[t][q] = max;
            }
        }
        
        double maxRevenue = 0;
        for (const auto& [q, revenue] : S_t_q.at(flight.q_t_k.size())) {
            maxRevenue = std::max(maxRevenue, revenue);
        }
        ExtractSolution(flight, S_t_q, f_t_x, iSolution);
        return maxRevenue;
    }

    void DynProgPDA::ExtractSolution(const Data::FlightData &flight, HashMap<int, HashMap<int, double>>& S_t_q, HashMap<int, 
        HashMap<int, Pair<String, double>>>& f_t_x, Vector<Pair<double, int>>& iSolution) const
    {
        int t = flight.q_t_k.size();
        int q = flight.Q;
        while (t > 0) {
            if (t == 1) {
                iSolution.emplace_back(f_t_x[t][q].second, q);
                q = 0;
                --t;
                break;
            }
            for (int x = 0; x <= q; ++x) {
                if (S_t_q[t][q] == S_t_q[t-1][q-x] + f_t_x[t][x].second*x) {
                    iSolution.emplace_back(f_t_x[t][x].second, x);
                    q -= x;
                    --t;
                    break;
                }
            }
        }
    }

    void DynProgPDA::Solve()
    {
        for (const auto& flight : _df.flights) {
            auto solution = Vector<Pair<double, int>>();
            const auto revenue = SolveSingleFlight(flight, solution);
            _results.emplace_back(revenue, solution);
        }
    }

    void DynProgPDA::ToCsv(const String& iFileName) const
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


