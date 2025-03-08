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
            const Vector<Pair<double, Vector<Pair<double, int>>>>& GetResults() const { return _results; }

        private:
            double SolveSingleFlight(const Data::FlightData& iFlight, Vector<Pair<double, int>>& iSolution) const;
            void ExtractSolution(const Data::FlightData& iFlight, HashMap<int, HashMap<int, double>>& S_t_q, 
                HashMap<int, HashMap<int, Pair<String, double>>>& f_t_x, Vector<Pair<double, int>>& iSolution) const;

            Data::FlightsDF _df;
            int N;
            Vector<Pair<double, Vector<Pair<double, int>>>> _results;
    };
}
