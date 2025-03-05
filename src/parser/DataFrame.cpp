#include "DataFrame.hpp"

namespace Parser 
{
    void DataFrame::read_csv(const std::string& filename)
    {
        csv::CSVReader reader(filename);
        bool first_row = true;

        for (csv::CSVRow& row : reader) {
            if (first_row) {
                // Первая строка - заголовки
                for (csv::CSVField& field : row) {
                    columns.push_back(field.get<>());
                }
                first_row = false;
            } 
            else {
                // Остальные строки - данные
                std::vector<std::string> row_data;
                for (csv::CSVField& field : row) {
                    row_data.push_back(field.get<>());
                }
                _data.push_back(row_data);
            }
        }
    }

}