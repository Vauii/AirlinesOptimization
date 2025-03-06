#include <iostream>
#include "parser/DataFrame.hpp"

int main() {
    Data::DataFrame df = Data::DataFrame("../data/predicted_pickups.csv", 300, 14);
    std::cout << df.data.size() << std::endl;

    for (const auto& flight : df.data) {
        std::cout << flight.date << " " << flight.origin << " " << flight.destination << " " << flight.Q << std::endl;
    }

    for (const auto& dayData : df.data[0].q_t_k) {
        if (dayData.first != 300) {
            continue;
        }
        for (const auto& bookingClassData : dayData.second) {
            std::cout << dayData.first << " " << bookingClassData.first << " " << bookingClassData.second << std::endl;
        }
    }

    return 0;
}
