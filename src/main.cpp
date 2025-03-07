#include <iostream>
#include "parser/FlightsDF.hpp"
#include "solvers/DynProgPDA.hpp"

int main() {
    Alg::DynProgPDA solver = Alg::DynProgPDA("../data/predicted_pickups.csv", 300, 14);
    solver.Solve();
    for (const auto& res : solver.GetResults()) {
        std::cout << res << std::endl;
    }
    solver.ToCsv("../results/DynProgPDA.csv");

    return 0;
}
