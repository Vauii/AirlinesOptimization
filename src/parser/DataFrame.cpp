#include <iostream>
#include "DataFrame.hpp"
#include "csv.hpp"

namespace Data 
{
    // T - number of days in the planning horizon
    // K - number of booking classes
    DataFrame::DataFrame(const std::string& iFilename, int T, int K)
    {
        csv::CSVReader reader(iFilename);

        int flightDataSize = T * K;
        int i = 1;

        for (csv::CSVRow& row : reader)
        {   

            if (i == 1) {
                data.emplace_back(row["FLTDATE"].get<std::string>(), row["ORIG"].get<std::string>(), row["DEST"].get<std::string>(), row["CAP"].get<int>());
                data.back().c_k.reserve(K);
                data.back().q_t_k.reserve(K);
            }
            data.back().c_k[row["BCL"].get<std::string>()] = row["price"].get<double>();
            data.back().q_t_k[T - row["DTD"].get<int>()][row["BCL"].get<std::string>()] = row["pickup"].get<int>();
            if (i == flightDataSize) {
                i = 1;
            }
            else {
                i++;
            }
        }
    }
}