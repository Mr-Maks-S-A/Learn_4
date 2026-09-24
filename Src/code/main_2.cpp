#include <iostream>
#include <set>
#include <functional>
#include <vector>
#include <Until/Input.hpp>

int main() {
    std::cout << "========================================\n";
    std::cout << "      ЗАДАЧА 2: УДАЛЕНИЕ ДУБЛИКАТОВ\n";
    std::cout << "========================================\n\n";

    // Получаем количество элементов с валидацией через Input.hpp
    int count = get_input<int>("Введите количество элементов (N >= 0): ", 0);

    if (count == 0) {
        std::cout << "\nКоличество элементов равно 0. Программа завершена.\n";
        return 0;
    }

    // std::greater<int> организует сортировку по убыванию внутри std::set
    std::set<int, std::greater<int>> numbers;
    std::vector<int> raw_inputs;
    raw_inputs.reserve(count);

    std::cout << "\n--- Ввод данных ---\n";
    for (int i = 0; i < count; ++i) {
        std::string prompt = "Элемент [" + std::to_string(i + 1) + "/" + std::to_string(count) + "]: ";
        int val = get_input<int>(prompt);
        raw_inputs.push_back(val);
        
        auto [it, inserted] = numbers.insert(val);
        if (inserted) {
            std::cout << "  -> Число " << val << " успешно добавлено в std::set\n";
        } else {
            std::cout << "  -> Число " << val << " уже присутствует (дубликат пропущен)\n";
        }
    }

    std::cout << "\n--- ДЕТАЛИЗАЦИЯ ---\n";
    std::cout << "Введено элементов всего : " << raw_inputs.size() << "\n";
    std::cout << "Уникальных элементов    : " << numbers.size() << "\n";
    std::cout << "Удалено дубликатов       : " << raw_inputs.size() - numbers.size() << "\n";

    std::cout << "\n[OUT]:\n";
    for (int num : numbers) {
        std::cout << num << "\n";
    }

    return 0;
}
