#pragma once

#include <vector>
#include "DataTypes/TypesDefinition.hpp"
#include "Parser/FlightsDF.hpp"

/*
    GreedyDTA - Discriminatory Ticket Allocation problem greedy solver
*/

namespace Alg
{
    class GreedyDTA
    {
        public:
            GreedyDTA(const String& iFilename, int T, int K);
            
            void Solve();
            void ToCsv(const String& iFileName) const;
            const Vector<double>& GetResults() const { return _results; }

        private:
            double SolveSingleFlight(const Data::FlightData& flight) const;

            Data::FlightsDF _df;
            int N;
            Vector<double> _results;
    };
}
