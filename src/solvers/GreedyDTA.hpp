#pragma once

#include <vector>
#include "parser/FlightsDF.hpp"

/*
    GreedyDTA - Discriminatory Ticket Allocation problem greedy solver
*/

namespace Alg
{
    class GreedyDTA
    {
        public:
            GreedyDTA(const std::string& iFilename, int T, int K);
            
            void Solve();
            void ToCsv(const std::string& iFileName) const;
            const std::vector<double>& GetResults() const { return _results; }

        private:
            double SolveSingleFlight(const Data::FlightData& flight) const;

            Data::FlightsDF _df;
            int N;
            std::vector<double> _results;
    };
}
