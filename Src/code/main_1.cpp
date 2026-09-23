#include <iostream>
#include <vector>

// Шаблонная функция для простых типов
template <typename T>
T sqr(T value) {
    return value * value;
}

// Шаблонная функция для векторов
template <typename T>
std::vector<T> sqr(const std::vector<T>& vec) {
    std::vector<T> result;
    result.reserve(vec.size());
    for (const auto& item : vec) {
        result.push_back(sqr(item));
    }
    return result;
}

// Вспомогательная функция для красивой печати вектора
template <typename T>
void print_vector(const std::vector<T>& vec) {
    for (size_t i = 0; i < vec.size(); ++i) {
        std::cout << vec[i] << (i + 1 < vec.size() ? ", " : "");
    }
}

int main() {
    std::cout << "=== Задача 1. Возведение в квадрат ===" << std::endl;

    // 1. Тест с простым числом
    int number = 4;
    std::cout << "[IN]:  " << number << std::endl;
    std::cout << "[OUT]: " << sqr(number) << std::endl << std::endl;

    // 2. Тест с вектором
    std::vector<int> vec = {-1, 4, 8};
    std::cout << "[IN]:  ";
    print_vector(vec);
    std::cout << std::endl;

    auto squared_vec = sqr(vec);
    std::cout << "[OUT]: ";
    print_vector(squared_vec);
    std::cout << std::endl << std::endl;

    return 0;
}
