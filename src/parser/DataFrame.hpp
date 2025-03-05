#pragma once
#include <iostream>
#include <vector>
#include <string>


namespace Data 
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
            std::vector<std::string> get_row(size_t index) const;
            // Получение столбца по имени
            std::vector<std::string> get_column(const std::string& column_name) const;
        };
}
