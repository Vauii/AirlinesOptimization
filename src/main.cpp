#include <iostream>
#include <iomanip>
#include <fstream>

#include "Parser/FlightsDF.hpp"
#include "Solvers/GreedyDTA.hpp"
#include "Solvers/DynProgPDA.hpp"
#include "Solvers/ModelSolverPDA.hpp"
#include "Solvers/ModelSolverIDA.hpp"

int main() {
    // Alg::ModelSolverPDA solver = Alg::ModelSolverPDA("../data/predicted_pickups.csv", 300, 14, "SCIP", true);
    // solver.Solve();
    // for (const auto& res : solver.GetResults()) {
    //     std::cout << std::fixed << std::setprecision(0) << res.first << std::endl;
    // }
    // solver.ToCsv("../results/ModelSolverPDA.csv");

    Alg::ModelSolverIDA solver = Alg::ModelSolverIDA("../data/predicted_pickups.csv", 300, 14, "SCIP", false, true);
    solver.Solve();
    for (const auto& res : solver.GetResults()) {
        std::cout << std::fixed << std::setprecision(0) << res.first << std::endl;
    }
    solver.ToCsv("../results/ModelSolverIDA.csv");

    // int Q = 10;
    // auto flight = Data::FlightData("-","-","-", Q);
    
    // flight.q_t_k[1] = {
    //     {"1", 7}, 
    //     {"2", 0}, 
    //     {"3", 0}
    // };
    // flight.q_t_k[2] = {
    //     {"1", 0}, 
    //     {"2", 2}, 
    //     {"3", 3}
    // };
    // flight.c_k = {
    //     {"1", 1.0},
    //     {"2", 4.0},
    //     {"3", 5.0}
    // };
    // auto solution = Alg::HashMap<int, Alg::HashMap<Alg::String, double>>();
    // double value = Alg::ModelSolverIDA::SolveSingleFlight(flight, solution, false, false, "SCIP");

    // Alg::DynProgPDA solver = Alg::DynProgPDA("../data/predicted_pickups.csv", 300, 14);
    // solver.Solve();
    // for (const auto& res : solver.GetResults()) {
    //     std::cout << std::fixed << std::setprecision(0) << res.first << std::endl;
    // }
    // solver.ToCsv("../results/DynProgPDA.csv");

    // Alg::GreedyDTA solver = Alg::GreedyDTA("../data/predicted_pickups.csv", 300, 14);
    // solver.Solve();
    // for (const auto& res : solver.GetResults()) {
    //     std::cout << std::fixed << std::setprecision(0) << res << std::endl;
    // }
    // solver.ToCsv("../results/GreedyDTA.csv");

    return 0;
}
