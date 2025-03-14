#pragma once

#include "DataTypes/TypesDefinition.hpp"
#include "Parser/FlightsDF.hpp"

#include "absl/flags/flag.h"
#include "absl/log/flags.h"
#include "ortools/base/init_google.h"
#include "ortools/base/logging.h"
#include "ortools/init/init.h"

/*
    ModelSolverPDA - Daily Price Assignment problem linear programming solver
*/

namespace Alg
{
    class ModelSolverPDA
    {
        public:
            ModelSolverPDA(const String& iFilename, int T, int K, const String& iSolverType = "SCIP");
            
            void Solve();
            void ToCsv(const String& iFileName) const;
            const Vector<Pair<double, Vector<Pair<double, int>>>>& GetResults() const { return _results; }

        private:
            double SolveSingleFlight(const Data::FlightData& iFlight, Vector<Pair<double, int>>& iSolution) const;

            // bool CheckSolution(const Data::FlightData& iFlight, const Vector<Pair<double, int>>& iSolution,
            //     const Vector<String>& iSolutionClasses, const double iRevenue) const;

            String _solverType;
            Data::FlightsDF _df;
            int N;
            Vector<Pair<double, Vector<Pair<double, int>>>> _results;
    };
}
