#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <Until/Input.hpp>

int main() {
    std::cout << "========================================\n";
    std::cout << "        ЗАДАЧА 1: ЧАСТОТА СИМВОЛОВ\n";
    std::cout << "========================================\n\n";

    // Использование Input.hpp для получения непустой строки
    std::string text = get_input<std::string>("[IN]: ", 1);

    std::cout << "\n--- ШАГ 1: Подсчет частоты символов через std::unordered_map ---\n";
    std::unordered_map<char, int> freq_map;
    for (char ch : text) {
        freq_map[ch]++;
    }

    std::cout << "Уникальных символов найдено: " << freq_map.size() << "\n";
    std::cout << "Содержимое хеш-таблицы (до сортировки):\n";
    for (const auto& [ch, count] : freq_map) {
        if (ch == ' ') {
            std::cout << "  ' ' (пробел) -> " << count << "\n";
        } else if (ch == '\t') {
            std::cout << "  '\\t' (табуляция) -> " << count << "\n";
        } else {
            std::cout << "  '" << ch << "' -> " << count << "\n";
        }
    }

    std::cout << "\n--- ШАГ 2: Перенос элементов в std::vector и сортировка ---\n";
    std::vector<std::pair<char, int>> freq_vec(freq_map.begin(), freq_map.end());

    // Сортировка по убыванию частоты
    std::sort(freq_vec.begin(), freq_vec.end(), [](const auto& a, const auto& b) {
        return a.second > b.second;
    });

    std::cout << "\n[OUT]:\n";
    for (const auto& [ch, count] : freq_vec) {
        std::cout << ch << ": " << count << "\n";
    }

    return 0;
}
