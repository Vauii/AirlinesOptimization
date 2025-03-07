#pragma once

#include <vector>
#include "parser/FlightsDF.hpp"

/*
    DynProgPDA - Daily Price Assignment
*/

namespace Alg
{
    class DynProgPDA
    {
        public:
            DynProgPDA(const std::string& iFilename, int T, int K);
            
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
