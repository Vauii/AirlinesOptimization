#pragma once
#include <vector>
#include <string>


namespace Data 
{
    class DataFrame 
    {
        private:
            std::vector<std::vector<std::string>> _data;
            std::vector<std::string> _columns;
        
        public:
            void ReadCSV(const std::string& filename);
        
            // Вывод всей таблицы
            void Print() const;
            // Получение строки по индексу
            std::vector<std::string> GetRow(size_t index) const;
            // Получение столбца по имени
            std::vector<std::string> GetColumn(const std::string& column_name) const;
    };
}
