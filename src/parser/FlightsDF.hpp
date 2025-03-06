#pragma once
#include <vector>
#include <string>
#include "FlightData.hpp"

namespace Data 
{
    class FlightsDF 
    {   
        // DataFrame with information about flights
        public:
            FlightsDF(const std::string& iFilename, int T, int K);

            size_t Size() const;

            std::vector<FlightData> flights;
    };
}
