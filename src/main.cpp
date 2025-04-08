#include <iostream>
#include <iomanip>
#include <fstream>

#include "Parser/FlightsDF.hpp"
#include "Solvers/GreedyDTA.hpp"
#include "Solvers/DynProgPDA.hpp"
#include "Solvers/ModelSolverPDA.hpp"
#include "Solvers/ModelSolverIDA.hpp"

int main() {
    Alg::ModelSolverPDA solver = Alg::ModelSolverPDA("../data/predicted_pickups_many_sfltno.csv", "SCIP", false);
    solver.Solve();
    for (const auto& res : solver.GetResults()) {
        std::cout << std::fixed << std::setprecision(0) << res.first << std::endl;
    }
    solver.ToCsv("../results/ModelSolverPDA.csv");

    // Alg::ModelSolverIDA solver = Alg::ModelSolverIDA("../data/predicted_pickups.csv", T, K, "SCIP", false, true);
    // solver.Solve();
    // for (const auto& res : solver.GetResults()) {
    //     std::cout << std::fixed << std::setprecision(0) << res.first << std::endl;
    // }
    // solver.ToCsv("../results/ModelSolverIDA.csv");

    // int Q = 20;
    // auto flight = Data::FlightData("-","-","-", Q);
    
    // flight.q_t_k[1] = {
    //     {"1", 1}, 
    //     {"2", 2}, 
    //     {"3", 1},
    //     {"4", 0},
    //     {"5", 2}
    // };
    // flight.q_t_k[2] = {
    //     {"1", 2}, 
    //     {"2", 0}, 
    //     {"3", 1},
    //     {"4", 2},
    //     {"5", 3}
    // };
    // flight.q_t_k[3] = {
    //     {"1", 3}, 
    //     {"2", 1}, 
    //     {"3", 0},
    //     {"4", 1},
    //     {"5", 3}
    // };
    // flight.q_t_k[4] = {
    //     {"1", 2}, 
    //     {"2", 0}, 
    //     {"3", 1},
    //     {"4", 1},
    //     {"5", 2}
    // };
    // flight.c_k = {
    //     {"1", 1.0},
    //     {"2", 2.0},
    //     {"3", 4.0},
    //     {"4", 6.0},
    //     {"5", 7.0}
    // };
    // auto solution = Alg::Vector<Alg::Pair<double, int>>();
    // double value = Alg::ModelSolverPDA::SolveSingleFlight(flight, solution, false, "SCIP");
    // auto solution = Alg::HashMap<int, Alg::HashMap<Alg::String, double>>();
    // double value = Alg::ModelSolverIDA::SolveSingleFlight(flight, solution, false, true, "SCIP");

    // Alg::DynProgPDA solver = Alg::DynProgPDA("../data/predicted_pickups.csv", T, K);
    // solver.Solve();
    // for (const auto& res : solver.GetResults()) {
    //     std::cout << std::fixed << std::setprecision(0) << res.first << std::endl;
    // }
    // solver.ToCsv("../results/DynProgPDA.csv");

    // Alg::GreedyDTA solver = Alg::GreedyDTA("../data/predicted_pickups.csv", T, K);
    // solver.Solve();
    // for (const auto& res : solver.GetResults()) {
    //     std::cout << std::fixed << std::setprecision(0) << res << std::endl;
    // }
    // solver.ToCsv("../results/GreedyDTA.csv");

    return 0;
}
