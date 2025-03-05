#pragma once
#include <iostream>
#include <vector>
#include <string>
#include "csv.hpp"

namespace Parser 
{
    class DataFrame {
        private:
            std::vector<std::vector<std::string>> _data;
            std::vector<std::string> columns;
        
        public:
            void read_csv(const std::string& filename);
        
            // Вывод всей таблицы
            void print() const;
        
            // Получение строки по индексу
            std::vector<std::string> get_row(size_t index) const {
                if (index >= _data.size()) {
                    throw std::out_of_range("Row index out of range");
                }
                return _data[index];
            }
        
            // Получение столбца по имени
            std::vector<std::string> get_column(const std::string& column_name) const {
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
        };
}
