#include <iostream>
#include <stdexcept>
#include <utility>
#include <string>
#include <Until/Input.hpp>

template <typename T>
class SimpleVector {
public:
    SimpleVector() : data_(nullptr), size_(0), capacity_(0) {}

    // Конструктор копирования
    SimpleVector(const SimpleVector& other) 
        : data_(other.capacity_ > 0 ? new T[other.capacity_] : nullptr),
          size_(other.size_),
          capacity_(other.capacity_) 
    {
        for (size_t i = 0; i < size_; ++i) {
            data_[i] = other.data_[i];
        }
    }

    // Оператор присваивания
    SimpleVector& operator=(const SimpleVector& other) {
        if (this != &other) {
            delete[] data_;
            size_ = other.size_;
            capacity_ = other.capacity_;
            data_ = capacity_ > 0 ? new T[capacity_] : nullptr;
            for (size_t i = 0; i < size_; ++i) {
                data_[i] = other.data_[i];
            }
        }
        return *this;
    }

    ~SimpleVector() {
        delete[] data_;
    }

    void push_back(const T& value) {
        if (size_ == capacity_) {
            size_t new_capacity = (capacity_ == 0) ? 1 : capacity_ * 2;
            std::cout << "  [РЕАЛЛОКАЦИЯ] Емкость исчерпана (" << capacity_ 
                      << "). Выделяется память под " << new_capacity << " элементов.\n";

            T* new_data = new T[new_capacity];

            for (size_t i = 0; i < size_; ++i) {
                new_data[i] = std::move(data_[i]);
            }

            delete[] data_;
            data_ = new_data;
            capacity_ = new_capacity;
        }

        data_[size_++] = value;
        std::cout << "  -> Добавлен элемент. Новые показатели: size = " 
                  << size_ << ", capacity = " << capacity_ << "\n";
    }

    T& at(size_t index) {
        if (index >= size_) {
            throw std::out_of_range("Ошибка: индекс " + std::to_string(index) + 
                                    " выходит за пределы размера контейнера (size = " + 
                                    std::to_string(size_) + ")");
        }
        return data_[index];
    }

    const T& at(size_t index) const {
        if (index >= size_) {
            throw std::out_of_range("Ошибка: индекс " + std::to_string(index) + 
                                    " выходит за пределы размера контейнера (size = " + 
                                    std::to_string(size_) + ")");
        }
        return data_[index];
    }

    size_t size() const {
        return size_;
    }

    size_t capacity() const {
        return capacity_;
    }

private:
    T* data_;
    size_t size_;
    size_t capacity_;
};

int main() {
    std::cout << "========================================\n";
    std::cout << "       ЗАДАЧА 3*: АНАЛОГ std::vector\n";
    std::cout << "========================================\n\n";

    SimpleVector<int> vec;

    while (true) {
        std::cout << "\n--- МЕНЮ УПРАВЛЕНИЯ КОНТЕЙНЕРОМ ---\n";
        std::cout << "1. Добавить элемент (push_back)\n";
        std::cout << "2. Получить элемент по индексу (at)\n";
        std::cout << "3. Вывести текущий размер (size)\n";
        std::cout << "4. Вывести текущую емкость (capacity)\n";
        std::cout << "5. Вывести все элементы\n";
        std::cout << "0. Выход\n";

        int choice = get_input<int>("Выберите действие [0-5]: ", 0, 5);

        if (choice == 0) {
            std::cout << "Завершение работы.\n";
            break;
        }

        switch (choice) {
            case 1: {
                int val = get_input<int>("Введите целое число для добавления: ");
                vec.push_back(val);
                break;
            }
            case 2: {
                if (vec.size() == 0) {
                    std::cout << "Контейнер пуст!\n";
                    break;
                }
                int idx = get_input<int>("Введите индекс [0 - " + std::to_string(vec.size() - 1) + "]: ");
                try {
                    std::cout << "Элемент по индексу " << idx << " = " << vec.at(idx) << "\n";
                } catch (const std::out_of_range& e) {
                    std::cout << e.what() << "\n";
                }
                break;
            }
            case 3:
                std::cout << "Текущий size = " << vec.size() << "\n";
                break;
            case 4:
                std::cout << "Текущий capacity = " << vec.capacity() << "\n";
                break;
            case 5: {
                if (vec.size() == 0) {
                    std::cout << "Контейнер пуст!\n";
                    break;
                }
                std::cout << "Элементы в контейнере: [ ";
                for (size_t i = 0; i < vec.size(); ++i) {
                    std::cout << vec.at(i) << (i + 1 < vec.size() ? ", " : "");
                }
                std::cout << " ]\n";
                break;
            }
        }
    }

    return 0;
}
