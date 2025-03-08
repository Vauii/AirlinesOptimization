#pragma once

#include <unordered_map>
#include "DataTypes/TypesDefinition.hpp"
#include <string>

namespace Data
{
    struct FlightData
    {
        FlightData(const String& iDate, const String& iOrigin, const String& iDestination, int iCapacity)
            : date(iDate), origin(iOrigin), destination(iDestination), Q(iCapacity) 
        {
        }

        HashMap<String, double> c_k; // c_k - the price of a ticket in class k.
        HashMap<int, HashMap<String, int>> q_t_k; // q^t_k - the number of passengers on day t in booking class k. 
        const int Q; // maximum sold tickets count
        String date, origin, destination;
    };
}
