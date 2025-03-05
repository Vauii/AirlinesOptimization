#include "DataFrame.hpp"
#include "csv.hpp"

namespace Data 
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

    void DataFrame::print() const
    {
        for (const auto& col : columns) {
            std::cout << col << "\t";
        }
        std::cout << "\n--------------------------------------\n";
        
        for (const auto& row : _data) {
            for (const auto& cell : row) {
                std::cout << cell << "\t";
            }
            std::cout << "\n";
        }
    }

    std::vector<std::string> DataFrame::get_row(size_t index) const
    {
        if (index >= _data.size()) {
            throw std::out_of_range("Row index out of range");
        }
        return _data[index];
    }

    std::vector<std::string> DataFrame::get_column(const std::string& column_name) const 
    {
        std::vector<std::string> column_data;
        auto it = std::find(columns.begin(), columns.end(), column_name);

        if (it == columns.end()) {
            throw std::invalid_argument("Column not found");
        }
        
        size_t col_index = std::distance(columns.begin(), it);
        for (const auto& row : _data) {
            column_data.push_back(row[col_index]);
        }
        return column_data;
    }

}