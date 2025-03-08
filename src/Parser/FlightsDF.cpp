#include "FlightsDF.hpp"

#include <iostream>
#include "csv.hpp"

namespace Data 
{
    // T - number of days in the planning horizon
    // K - number of booking classes
    FlightsDF::FlightsDF(const String& iFilename, int T, int K)
    {
        csv::CSVReader reader(iFilename);

        int flightDataSize = T * K;
        int i = 1;

        for (csv::CSVRow& row : reader)
        {   

            if (i == 1) {
                flights.emplace_back(row["FLTDATE"].get<String>(), row["ORIG"].get<String>(), row["DEST"].get<String>(), row["CAP"].get<int>());
                flights.back().c_k.reserve(K);
                flights.back().q_t_k.reserve(K);
            }
            flights.back().c_k[row["BCL"].get<String>()] = row["price"].get<double>();
            flights.back().q_t_k[T - row["DTD"].get<int>()][row["BCL"].get<String>()] = row["pickup"].get<int>();
            if (i == flightDataSize) {
                i = 1;
            }
            else {
                i++;
            }
        }
    }

    size_t FlightsDF::Size() const
    {
        return flights.size();
    }
}