#pragma once
#include <vector>
#include <string>
#include "FlightData.hpp"

namespace Data 
{
    class DataFrame 
    {   
        public:
            DataFrame(const std::string& filename, int T, int K);

            std::vector<FlightData> data;
    };
}
