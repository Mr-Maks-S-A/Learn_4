#include <iostream>
#include <vector>
#include <set>
#include <list>
#include <string>
#include <algorithm>

#include <Until/Input.hpp> 

// ==========================================
// Задача 1: Уникальный вектор
// ==========================================
template <typename T>
void make_unique(std::vector<T>& vec) {
    // 1. Сортируем вектор, чтобы одинаковые элементы оказались рядом
    std::sort(vec.begin(), vec.end());
    
    // 2. Сдвигаем дубликаты в конец вектора
    auto it = std::unique(vec.begin(), vec.end());
    
    // 3. Физически удаляем дубликаты из вектора
    vec.erase(it, vec.end());
}

// ==========================================
// Задача 2: Печать контейнера
// ==========================================
template <typename Container>
void print_container(const Container& container) {
    // Используем константные итераторы для универсального обхода
    for (auto it = container.cbegin(); it != container.cend(); ++it) {
        std::cout << *it;
        
        // Красивое разделение элементов запятой 
        auto next_it = it;
        if (++next_it != container.cend()) {
            std::cout << ", ";
        }
    }
    std::cout << std::endl;
}

int main() {
    // --- Тестирование Задачи 1 ---
    std::cout << "=== Задача 1. Уникальный вектор ===" << std::endl;
    std::vector<int> numbers = { 1, 1, 2, 5, 6, 1, 2, 4 };
    
    std::cout << "[IN]: ";
    print_container(numbers);
    
    make_unique(numbers);
    
    std::cout << "[OUT]: ";
    print_container(numbers);
    std::cout << std::endl;

    // --- Тестирование Задачи 2 ---
    std::cout << "=== Задача 2. Печать контейнера ===" << std::endl;
    
    std::set<std::string> test_set = { "one", "two", "three", "four" };
    std::cout << "std::set:   ";
    print_container(test_set); // Выведет: four, one, three, two (отсортировано set-ом)

    std::list<std::string> test_list = { "one", "two", "three", "four" };
    std::cout << "std::list:  ";
    print_container(test_list); // Выведет: one, two, three, four

    std::vector<std::string> test_vector = { "one", "two", "three", "four" };
    std::cout << "std::vector: ";
    print_container(test_vector); // Выведет: one, two, three, four

    std::cout << "\n=== Интерактивный ввод с валидатором ===" << std::endl;
    int count = get_input<int>("Сколько чисел вы хотите ввести в новый вектор? (1-10): ", 1, 10);
    
    std::vector<int> user_vec;
    for (int i = 0; i < count; ++i) {
        int val = get_input<int>("Введите число " + std::to_string(i + 1) + ": ");
        user_vec.push_back(val);
    }

    std::cout << "Ваш исходный вектор: ";
    print_container(user_vec);

    make_unique(user_vec);

    std::cout << "Вектор без дубликатов: ";
    print_container(user_vec);

    return 0;
}
