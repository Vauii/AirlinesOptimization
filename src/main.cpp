#include <iostream>
#include <iomanip>
#include <fstream>

#include "Parser/FlightsDF.hpp"
#include "Solvers/GreedyDTA.hpp"
#include "Solvers/DynProgPDA.hpp"
#include "Solvers/ModelSolverPDA.hpp"

int main() {
    Alg::ModelSolverPDA solver = Alg::ModelSolverPDA("../data/predicted_pickups.csv", 300, 14, "CBC");
    solver.Solve();
    for (const auto& res : solver.GetResults()) {
        std::cout << std::fixed << std::setprecision(0) << res.first << std::endl;
    }
    solver.ToCsv("../results/ModelSolverPDA.csv");

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
