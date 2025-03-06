#include <iostream>
#include "parser/DataFrame.hpp"

int main() {
    Data::DataFrame df;
    df.ReadCSV("../data/predicted_pickups.csv");

    std::cout << "\n🔹 Первая строка: ";
    for (const auto& cell : df.GetRow(0)) {
        std::cout << cell << " ";
    }
    std::cout << "\n";

    std::cout << "\n🔹 Вторая строка: ";
    for (const auto& cell : df.GetRow(1)) {
        std::cout << cell << " ";
    }
    std::cout << "\n";

    std::cout << "\n🔹 Столбец 'price': \n";
    int i = 0;
    for (const auto& val : df.GetColumn("price")) {
        std::cout << val << " \n";
        i++;
        if (i > 5)
            break;
    }
    std::cout << "\n";

    return 0;
}
