#include <iostream>
#include <iomanip>
#include <fstream>

#include "parser/FlightsDF.hpp"
#include "solvers/GreedyDTA.hpp"

int main() {
    // Alg::DynProgPDA solver = Alg::DynProgPDA("../data/predicted_pickups.csv", 300, 14);
    // solver.Solve();
    // for (const auto& res : solver.GetResults()) {
    //     std::cout << res << std::endl;
    // }
    // solver.ToCsv("../results/DynProgPDA.csv");

    Alg::GreedyDTA solver = Alg::GreedyDTA("../data/predicted_pickups.csv", 300, 14);
    solver.Solve();
    for (const auto& res : solver.GetResults()) {
        std::cout << std::fixed << std::setprecision(0) << res << std::endl;
    }
    solver.ToCsv("../results/GreedyDTA.csv");

    return 0;
}
