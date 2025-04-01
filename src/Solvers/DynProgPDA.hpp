#pragma once

#include "DataTypes/TypesDefinition.hpp"
#include "Parser/FlightsDF.hpp"

/*
    DynProgPDA - Price Daily Assignment problem DP solver
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

            static HashMap<int, String> SolveSingleFlight(const Data::FlightData& iFlight, const double iPriceEpsilon = 1e-6);

        private:
            static double SolveSingleFlight(const Data::FlightData& iFlight, Vector<Pair<double, int>>& iSolution);
            static void ExtractSolution(const Data::FlightData& iFlight, HashMap<int, HashMap<int, double>>& S_t_q, 
                HashMap<int, HashMap<int, Pair<String, double>>>& f_t_x, Vector<Pair<double, int>>& iSolution,
                Vector<String>& iSolutionClasses);
            static bool CheckSolution(const Data::FlightData& iFlight, const Vector<Pair<double, int>>& iSolution,
                const Vector<String>& iSolutionClasses, const double iRevenue);

            Data::FlightsDF _df;
            int N;
            Vector<Pair<double, Vector<Pair<double, int>>>> _results;
    };
}
