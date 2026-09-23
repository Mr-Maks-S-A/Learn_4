#include <iostream>
#include <vector>
#include <utility>

template <typename T>
class Table {
private:
    std::vector<std::vector<T>> data_;
    size_t rows_;
    size_t cols_;

public:
    Table(size_t rows, size_t cols) : rows_(rows), cols_(cols) {
        data_.resize(rows, std::vector<T>(cols));
    }

    std::vector<T>& operator[](size_t index) {
        return data_[index];
    }

    const std::vector<T>& operator[](size_t index) const {
        return data_[index];
    }

    std::pair<size_t, size_t> Size() const {
        return {rows_, cols_};
    }
};

int main() {
    std::cout << "=== Задача 2. Таблица ===" << std::endl;
    // Создаём таблицу 2x3
    auto test = Table<int>(2, 3);
    
    // Заполняем некоторые значения
    test[0][0] = 4;
    test[0][1] = 7;
    test[1][2] = 12;

    // Выводим размерность
    auto size = test.Size();
    std::cout << "Размер созданной таблицы: " << size.first << " строк(и), " 
              << size.second << " столбца(ов)." << std::endl << std::endl;
    // Выводим сгенерированную таблицу полностью
    std::cout << "Содержимое таблицы:" << std::endl;
    for (size_t i = 0; i < size.first; ++i) {
        std::cout << "Строка " << i << ": ";
        for (size_t j = 0; j < size.second; ++j) {
            std::cout << "[" << test[i][j] << "] ";
        }
        std::cout << std::endl;
    }

    std::cout << "\nПроверка прямого доступа test[0][0]: " << test[0][0];
    std::cout << "\nПроверка прямого доступа test[0][1]: " << test[0][1];
    std::cout << "\nПроверка прямого доступа test[1][0]: " << test[1][0];
  
    return 0;
}
