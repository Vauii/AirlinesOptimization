## Установка зависимостей

1. Установите библиотеку `csv-parser`:
   ```bash
   cd ~
   git clone https://github.com/vincentlaucsb/csv-parser.git
   sudo cp csv-parser/single_include/csv.hpp /usr/local/include/
   ```

2. Соберите `csv-parser` и добавьте в PATH:
   ```bash
   mkdir build
   cd build
   cmake ..
   make
   cd ..
   sudo cp single_include/csv.hpp /usr/local/include/
   ```

## Сборка проекта
1. Сборка
   ```bash
   mkdir build
   cd build
   cmake ..
   make
   ```
2. Запуск
   ```bash
   cd ./airlines
   ```