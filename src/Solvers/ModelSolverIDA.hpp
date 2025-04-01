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
        IDA - Inventory Daily Assignment problem linear programming solver
    */
    class ModelSolverIDA
    {
        public:
            /** @brief Constructor of a ModelSolver. 
             *  @details Linear programming solver including MIP. 
             *  @param iFilename - Path to dataset. 
             *  @param T - Time moments count in each sample. 
             *  @param K - Ticket pricing classes count. 
             *  @param iSolverType - SCIP or CBC. 
             *  @param iUseRelaxation - Relaxe variables if true. 
            */
            ModelSolverIDA(const String& iFilename, int T, int K, const String& iSolverType = "SCIP",
                 bool iUseRelaxation = false, bool iUseDynProgResults = false);
            
            void Solve();
            static double SolveSingleFlight(const Data::FlightData& iFlight, HashMap<int, HashMap<String,
                 double>>& iSolution, bool iUseRelaxation, bool iUseDynProgResults, String iSolverType);
            void ToCsv(const String& iFileName) const;
            const Vector<Pair<double, HashMap<int, HashMap<String, double>>>>& GetResults() const { return _results; }

        private:
            double SolveFlight(const Data::FlightData& iFlight, HashMap<int, HashMap<String, double>>& iSolution) const;
            String _solverType;
            Data::FlightsDF _df;
            int N;
            bool _useRelaxation;
            bool _useDynProgResults;
            Vector<Pair<double, HashMap<int, HashMap<String, double>>>> _results;
    };
}
