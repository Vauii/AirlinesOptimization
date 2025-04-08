#pragma once

#include "DataTypes/TypesDefinition.hpp"
#include "Parser/FlightsDF.hpp"

#include "absl/flags/flag.h"
#include "absl/log/flags.h"
#include "ortools/base/init_google.h"
#include "ortools/base/logging.h"
#include "ortools/init/init.h"

namespace Alg
{
    /*
        PDA - Price Daily Assignment problem linear programming solver
    */
    class ModelSolverPDA
    {
        public:
            /** @brief Constructor of a ModelSolver. 
             *  @details Linear programming solver including MIP. 
             *  @param iFilename - Path to dataset.
             *  @param iSolverType - SCIP or CBC. 
             *  @param iUseRelaxation - Relaxe variables if true. 
            */
            ModelSolverPDA(const String& iFilename, const String& iSolverType = "SCIP", bool iUseRelaxation = false);
            
            static double SolveSingleFlight(const Data::FlightData& iFlight,
                Vector<Pair<double, int>>& iSolution, bool iUseRelaxation = false, String iSolverType = "SCIP");
            
            void Solve();
            void ToCsv(const String& iFileName) const;
            const Vector<Pair<double, Vector<Pair<double, int>>>>& GetResults() const { return _results; }

        private:
            double SolveFlight(const Data::FlightData& iFlight, Vector<Pair<double, int>>& iSolution) const;

            String _solverType;
            Data::FlightsDF _df;
            bool _useRelaxation;
            Vector<Pair<double, Vector<Pair<double, int>>>> _results;
    };
}
