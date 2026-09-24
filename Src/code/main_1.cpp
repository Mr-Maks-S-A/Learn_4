#include <iostream>
#include <vector>
#include <string>
#include <utility>

template <typename T>
void move_vectors(std::vector<T>& src, std::vector<T>& dest) {
    dest = std::move(src); // Перемещаем ресурсы вектора src в dest
    // src остается в валидном, но пустом состоянии
}

int main() {
    std::vector<std::string> one = { "test_string1", "test_string2" };
    std::vector<std::string> two;

    move_vectors(one, two);

    std::cout << "Vector 'one' size: " << one.size() << "\n";
    std::cout << "Vector 'two' content: ";
    for (const auto& str : two) {
        std::cout << str << " ";
    }
    std::cout << "\n";

    return 0;
}
