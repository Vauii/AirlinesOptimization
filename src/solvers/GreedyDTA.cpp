#include "GreedyDTA.hpp"

#include <iostream>
#include <algorithm>
#include <iterator>
#include <fstream>

namespace Alg
{
    GreedyDTA::GreedyDTA(const std::string &iFilename, int T, int K)
        : _df(iFilename, T, K)
    {
        N = _df.Size() / (T * K);
        _results.reserve(N);
    }

    double GreedyDTA::SolveSingleFlight(const Data::FlightData& flight) const
    {
        std::unordered_map<std::string, int> q_k; // q_k - sum number of clients for each class
        for (const auto& t : flight.q_t_k) {
            for (const auto& k : t.second) {
                q_k[k.first] += k.second;
            }
        }

        std::vector<std::pair<std::string, int>> q_k_sorted(
            std::make_move_iterator(q_k.begin()),
            std::make_move_iterator(q_k.end()));
        std::sort(q_k_sorted.begin(), q_k_sorted.end(), [](const auto& a, const auto& b) {
            return a.second > b.second;
        });

        double revenue = 0.0;
        int ticketsRemaining = flight.Q;

        for (const auto& [k, clientsNum] : q_k_sorted) {
            int ticketsToSell = std::min(clientsNum, ticketsRemaining);
            revenue += ticketsToSell * flight.c_k.at(k);
            ticketsRemaining -= ticketsToSell;
            if (ticketsRemaining == 0) {
                break;
            }
        }
    
        return revenue;
    }

    void GreedyDTA::Solve()
    {
        for (const auto& flight : _df.flights) {
            _results.push_back(SolveSingleFlight(flight));
        }
    }

    void GreedyDTA::ToCsv(const std::string& iFileName) const
    {   
        std::ofstream outputFile(iFileName);

        if (!outputFile.is_open()) {
            std::cerr << "Failed to open the file!\n";
            return;
        }

        outputFile << "FLTDATE,ORIG,DEST,Score\n";
        for (size_t i = 0; i < _results.size(); ++i) {
            outputFile << _df.flights[i].date << ","
                << _df.flights[i].origin << ","
                << _df.flights[i].destination << ","
                << _results[i] << "\n";
        }
        outputFile.close();

        std::cout << "Results saved to " << iFileName << std::endl;
    }
}


