#include "DynProgPDA.hpp"

#include <iostream>
#include <iterator>
#include <iomanip>
#include <fstream>
#include <climits>
#include <cassert>

namespace Alg
{
    DynProgPDA::DynProgPDA(const String &iFilename, int T, int K)
        : _df(iFilename, T, K)
    {
        N = _df.Size() / (T * K);
        _results.reserve(N);
    }

    double DynProgPDA::SolveSingleFlight(const Data::FlightData& iFlight, Vector<Pair<double, int>>& iSolution)
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
        
        HashMap<int, HashMap<int, Pair<String, double>>> f_t_x;
        HashMap<int, Set<int>> Q_t_c;
        for (const auto& [t, _]: iFlight.q_t_k) {
            Q_t_c[t] = Set<int>();
            for (const auto& [k, c_k] : iFlight.c_k) {
                if (iFlight.q_t_k.at(t).at(k) == 0) {
                    continue;
                }
                int potentialClients = q_t_c(t, c_k);
                Q_t_c[t].insert(potentialClients);
                f_t_x[t][potentialClients].second = c_k;
                f_t_x[t][potentialClients].first = k;
            }
            Q_t_c[t].insert(0);
            f_t_x[t][0].second = INT_MAX;
            f_t_x[t][0].first = "";
        }

        HashMap<int, HashMap<int, double>> S_t_q;
        S_t_q.reserve(iFlight.q_t_k.size());
        auto CalculateMax = [&](const double max, const int t, const int q, const int x) {
            if (t == 1) {
                return std::max(max, f_t_x[t][x].second*x);
            }
            return std::max(max, f_t_x[t][x].second*x + S_t_q[t-1][q-x]);
        };

        // TODO: сделать итерацию по t нормально
        for (int t = 1; t <= iFlight.q_t_k.size(); ++t) {
            // std::cout << "t = " << t << std::endl;
            for (int q = 0; q <= iFlight.Q; ++q) {
                double max = 0;
                for (const auto& x: Q_t_c.at(t)) {
                    if (x > q) {
                        break;
                    }
                    max = CalculateMax(max, t, q, x);
                }
                S_t_q[t][q] = max;
            }
        }
        
        double maxRevenue = 0;
        for (const auto& [q, revenue] : S_t_q.at(iFlight.q_t_k.size())) {
            maxRevenue = std::max(maxRevenue, revenue);
        }
        Vector<String> solutionClasses;
        ExtractSolution(iFlight, S_t_q, f_t_x, iSolution, solutionClasses);
        assert(CheckSolution(iFlight, iSolution, solutionClasses, maxRevenue));
        return maxRevenue;
    }

    void DynProgPDA::ExtractSolution(const Data::FlightData &flight, HashMap<int, HashMap<int, double>>& S_t_q, 
        HashMap<int, HashMap<int, Pair<String, double>>>& f_t_x, Vector<Pair<double, int>>& iSolution,
        Vector<String>& iSolutionClasses)
    {
        int t = flight.q_t_k.size();
        int q = flight.Q;
        while (t > 0 && q > 0) {
            for (int x = 0; x <= q; ++x) {
                double prev_S_t_q = t == 1 ? 0 : S_t_q[t-1][q-x]; 
                if (S_t_q[t][q] == prev_S_t_q + f_t_x[t][x].second*x && x > 0) {
                    iSolution.emplace_back(f_t_x[t][x].second, x);
                    iSolutionClasses.emplace_back(f_t_x[t][x].first);
                    q -= x;
                    --t;
                    break;
                }
            }
        }
    }

    bool Alg::DynProgPDA::CheckSolution(const Data::FlightData &iFlight, const Vector<Pair<double, int>> &iSolution,
        const Vector<String> &iSolutionClasses, const double iRevenue)
    {
        if (iSolution.size() != iFlight.q_t_k.size() || iSolutionClasses.size() != iFlight.q_t_k.size()) {
            return false;
        }
        int soldTickets = 0;
        double revenue = 0, solutionRevenue = 0;

        for (int t = iFlight.q_t_k.size(); t >= 1; --t) {
            int i = iFlight.q_t_k.size() - t;
            const auto& [c, x] = iSolution[i];
            solutionRevenue += c*x;

            int potentialClients = 0;
            for (const auto& [k, c_k] : iFlight.c_k) {
                if (c_k >= c) {
                    potentialClients += iFlight.q_t_k.at(t).at(k);
                }
            }
            if (iSolutionClasses[i] == "")
                continue;
            if (x != potentialClients) {
                return false;
            }
            soldTickets += potentialClients;
            revenue += c*potentialClients;
        }
        return soldTickets <= iFlight.Q && revenue == solutionRevenue && solutionRevenue == iRevenue;
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

    HashMap<int, String> DynProgPDA::SolveSingleFlight(const Data::FlightData &iFlight, const double iPriceEpsilon)
    {
        auto solution = Vector<Pair<double, int>>();
        const auto revenue = SolveSingleFlight(iFlight, solution);
        HashMap<int, String> solutionClasses;
        const int T = iFlight.q_t_k.size();
        solutionClasses.reserve(T);
        for (int i = solution.size() - 1; i >= 0; --i) {
            for (const auto& [k, c_k] : iFlight.c_k) {
                if (std::abs(c_k - solution[i].first) < iPriceEpsilon) {
                    solutionClasses[T - i] = k;
                    break;
                }
            }
        }
        return solutionClasses;
    }
}


