#include <iostream>
#include "parser/FlightsDF.hpp"
#include "solvers/GreedyDTA.hpp"

int main() {
    Alg::GreedyDTA solver = Alg::GreedyDTA("../data/predicted_pickups.csv", 300, 14);
    solver.Solve();
    for (const auto& res : solver.GetResults()) {
        std::cout << res << std::endl;
    }
    solver.ToCsv("../results/greedyDTA.csv");

    return 0;
}
