#include <iostream>
#include <string>
#include <algorithm>
#include <utility>

class big_integer {
private:
    std::string digits; // Число хранится в виде строки

public:
    // 1. Конструкторы по умолчанию и от строки
    big_integer() : digits("0") {}
    big_integer(const std::string& val) : digits(val) {
        // Удаляем ведущие нули (кроме случая, когда число "0")
        size_t first_non_zero = digits.find_first_not_of('0');
        if (first_non_zero != std::string::npos) {
            digits = digits.substr(first_non_zero);
        } else {
            digits = "0";
        }
    }

    // 2. Копирующий конструктор и копирующий оператор присваивания
    big_integer(const big_integer& other) = default;
    big_integer& operator=(const big_integer& other) = default;

    // 3. Перемещающий конструктор
    big_integer(big_integer&& other) noexcept 
        : digits(std::move(other.digits)) {
        other.digits = "0";
    }

    // 4. Перемещающий оператор присваивания
    big_integer& operator=(big_integer&& other) noexcept {
        if (this != &other) {
            digits = std::move(other.digits);
            other.digits = "0";
        }
        return *this;
    }

    // 5. Оператор сложения двух больших чисел
    big_integer operator+(const big_integer& other) const {
        std::string result = "";
        int i = static_cast<int>(digits.length()) - 1;
        int j = static_cast<int>(other.digits.length()) - 1;
        int carry = 0;

        while (i >= 0 || j >= 0 || carry > 0) {
            int sum = carry;
            if (i >= 0) sum += digits[i--] - '0';
            if (j >= 0) sum += other.digits[j--] - '0';
            
            carry = sum / 10;
            result.push_back((sum % 10) + '0');
        }

        std::reverse(result.begin(), result.end());
        return big_integer(result);
    }

    // 6. Оператор умножения на число (например, int)
    big_integer operator*(int num) const {
        if (num == 0 || digits == "0") {
            return big_integer("0");
        }

        std::string result = "";
        int carry = 0;

        for (int i = static_cast<int>(digits.length()) - 1; i >= 0; --i) {
            long long current = static_cast<long long>(digits[i] - '0') * num + carry;
            carry = static_cast<int>(current / 10);
            result.push_back(static_cast<char>((current % 10) + '0'));
        }

        while (carry > 0) {
            result.push_back(static_cast<char>((carry % 10) + '0'));
            carry /= 10;
        }

        std::reverse(result.begin(), result.end());
        return big_integer(result);
    }

    // Оператор вывода в поток
    friend std::ostream& operator<<(std::ostream& os, const big_integer& bi) {
        os << bi.digits;
        return os;
    }
};

int main() {
    // Проверка согласно условию задачи
    auto number1 = big_integer("114575");
    auto number2 = big_integer("78524");
    
    auto result = number1 + number2;
    std::cout << "Sum: " << result << std::endl; // Ожидается: 193099

    // Проверка умножения
    auto mult_result = number1 * 25;
    std::cout << "Mult: " << mult_result << std::endl; // 114575 * 25 = 2864375

    // Проверка перемещения
    big_integer moved_num = std::move(number1);
    std::cout << "Moved num: " << moved_num << std::endl;
    std::cout << "Original num1 after move: " << number1 << std::endl; // Остался "0"

    return 0;
}
