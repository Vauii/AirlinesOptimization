#include "FlightsDF.hpp"

#include <iostream>
#include "csv.hpp"

namespace Data 
{
    FlightsDF::FlightsDF(const String& iFilename)
    {
        csv::CSVReader reader(iFilename);
        bool IsFirstRow = true;
        String prevDate;
        String prevOrigin;
        String prevDestination;
        HashMap<int, HashMap<String, int>> q_DTD_k; // q^DTD_k - the number of passengers on DTD in booking class k, where DTD stands for days till department
        int T = 0;

        for (csv::CSVRow& row : reader)
        {   
            if (IsFirstRow)
            {
                prevDate = row["FLTDATE"].get<String>();
                prevOrigin = row["ORIG"].get<String>();
                prevDestination = row["DEST"].get<String>();
                flights.emplace_back(prevDate, prevOrigin, prevDestination, row["CAP"].get<int>());
                IsFirstRow = false;
            }
            const auto currDate = row["FLTDATE"].get<String>();
            const auto currOrigin = row["ORIG"].get<String>();
            const auto currDestination = row["DEST"].get<String>();
            if (prevDate != currDate || prevOrigin != currOrigin || prevDestination != currDestination){
                T += 1;
                for (const auto& [dtd, q_k] : q_DTD_k)
                {
                    flights.back().q_t_k[T - dtd] = q_k;
                }
                q_DTD_k.clear();
                T = 0;
                prevDate = currDate;
                prevOrigin = currOrigin;
                prevDestination = currDestination;
                flights.emplace_back(prevDate, prevOrigin, prevDestination, row["CAP"].get<int>());
            }
            const auto ticketClass = row["BCL"].get<String>();
            const auto price = row["price"].get<double>();
            const auto pickup = row["pickup"].get<int>();
            flights.back().c_k[ticketClass] = price;
            int dtd = row["DTD"].get<int>();
            if (dtd > T) 
                T = dtd;
            q_DTD_k[dtd][ticketClass] = pickup;
        }
        T += 1;
        for (const auto& [dtd, q_k] : q_DTD_k)
        {
            flights.back().q_t_k[T - dtd] = q_k;
        }
    }

    size_t FlightsDF::Size() const
    {
        return flights.size();
    }
}