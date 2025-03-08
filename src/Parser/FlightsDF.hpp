#pragma once
#include <vector>
#include <string>
#include "DataTypes/TypesDefinition.hpp"
#include "FlightData.hpp"

namespace Data 
{
    class FlightsDF 
    {   
        // DataFrame with information about flights
        public:
            FlightsDF(const String& iFilename, int T, int K);

            size_t Size() const;

            Vector<FlightData> flights;
    };
}
