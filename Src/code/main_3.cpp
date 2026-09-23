#include <iostream>
#include <vector>
#include <algorithm>

template <typename T = int>
class DivisibleCounter {
private:
    T sum_{0};
    size_t count_{0};
    T divisor_;

public:
    // По умолчанию ищем числа, кратные 3
    explicit DivisibleCounter(T divisor = 3) : divisor_(divisor) {}

    // Шаблонный оператор вызова функтора
    template <typename ElementT>
    void operator()(const ElementT& number) {
        // Подходит для целых типов (для плавающей точки можно адаптировать при необходимости)
        if (static_cast<long long>(number) % static_cast<long long>(divisor_) == 0) {
            sum_ += static_cast<T>(number);
            ++count_;
            std::cout << "  [+] Число " << number << " делится на " << divisor_ 
                      << " | Текущая сумма: " << sum_ << ", кол-во: " << count_ << std::endl;
        } else {
            std::cout << "  [-] Число " << number << " не делится на " << divisor_ << std::endl;
        }
    }

    T get_sum() const { return sum_; }
    size_t get_count() const { return count_; }
};

int main() {
    std::cout << "=== Задача 3. Универсальный счётчик (Шаблоны) ===" << std::endl;

    // --- Пример 1: Стандартный вектор int ---
    std::vector<int> numbers = {4, 1, 3, 6, 25, 54};

    std::cout << "\n[IN]: ";
    for (int n : numbers) std::cout << n << " ";
    std::cout << std::endl << "\nОбработка элементов (деление на 3):" << std::endl;

    // Передаём шаблонный функтор в std::for_each
    auto counter_int = std::for_each(numbers.begin(), numbers.end(), DivisibleCounter<int>(3));

    std::cout << "\n[OUT]: get_sum()   = " << counter_int.get_sum() << std::endl;
    std::cout << "[OUT]: get_count() = " << counter_int.get_count() << std::endl;

    // --- Пример 2: Демонстрация универсальности (тип long long и другой делитель) ---
    std::cout << "\n----------------------------------------" << std::endl;
    std::cout << "Проверка универсальности: vector<long long> и делитель 5" << std::endl;

    std::vector<long long> big_numbers = {10, 12, 25, 33, 50};
    auto counter_long = std::for_each(big_numbers.begin(), big_numbers.end(), DivisibleCounter<long long>(5));

    std::cout << "\n[OUT]: get_sum()   = " << counter_long.get_sum() << std::endl;
    std::cout << "[OUT]: get_count() = " << counter_long.get_count() << std::endl;

    return 0;
}
