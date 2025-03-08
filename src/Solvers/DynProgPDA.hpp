#pragma once

#include <vector>
#include "DataTypes/TypesDefinition.hpp"
#include "Parser/FlightsDF.hpp"

/*
    DynProgPDA - Daily Price Assignment problem DP solver
*/

namespace Alg
{
    class DynProgPDA
    {
        public:
            DynProgPDA(const String& iFilename, int T, int K);
            
            void Solve();
            void ToCsv(const String& iFileName) const;
            const std::vector<double>& GetResults() const { return _results; }

        private:
            double SolveSingleFlight(const Data::FlightData& flight) const;
            std::vector<String> ExtractSolution(const Data::FlightData& flight, HashMap<int, HashMap<int, double>>& S_t_q) const;

            Data::FlightsDF _df;
            int N;
            std::vector<double> _results;
    };
}
