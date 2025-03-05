#include <iostream>
#include "parser/DataFrame.hpp"

int main() {
    DataFrame df;
    df.read_csv("../data/predicted_pickups.csv");
    
    std::cout << "📊 DataFrame content:\n";
    df.print();

    std::cout << "\n🔹 Вторая строка: ";
    for (const auto& cell : df.get_row(1)) {
        std::cout << cell << " ";
    }
    std::cout << "\n";

    std::cout << "\n🔹 Столбец 'Age': ";
    for (const auto& val : df.get_column("Age")) {
        std::cout << val << " ";
    }
    std::cout << "\n";

    return 0;
}
