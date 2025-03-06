#pragma once

#include <unordered_map>
#include <string>

namespace Data
{
    struct FlightData
    {
        FlightData(std::string& iDate, std::string& iOrigin, std::string& iDestination, int iCapacity)
            : date(iDate), origin(iOrigin), destination(iDestination), Q(iCapacity) 
        {
        }

        std::unordered_map<int, double> c_k; // c_k - the price of a ticket in class k.
        std::unordered_map<int, std::unordered_map<int, int>> q_t_k; // q^t_k - the number of passengers on day t in booking class k. 
        const int Q; // maximum sold tickets count
        std::string date, origin, destination;
    };
}
