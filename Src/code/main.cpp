#include <iostream>
#include <stdexcept>
#include <utility>

// ============================================================================
// РЕАЛИЗАЦИЯ УМНОГО МАССИВА (RAII)
// ============================================================================

class smart_array {
private:
    int* data;          // Указатель на динамический массив
    size_t capacity;    // Максимальная вместимость (выделенный размер)
    size_t size;        // Текущее количество добавленных элементов

public:
    // 1. Конструктор с указанием емкости
    explicit smart_array(size_t capacity) 
        : capacity(capacity), size(0) {
        if (capacity == 0) {
            throw std::invalid_argument("Емкость массива должна быть больше 0");
        }
        data = new int[capacity];
    }

    // 2. Деструктор (RAII: освобождение ресурсов)
    ~smart_array() {
        delete[] data;
    }

    // 3. Конструктор копирования (Задача 2)
    smart_array(const smart_array& other) 
        : capacity(other.capacity), size(other.size) {
        data = new int[capacity];
        for (size_t i = 0; i < size; ++i) {
            data[i] = other.data[i];
        }
    }

    // 4. Оператор присваивания (Задача 2)
    smart_array& operator=(const smart_array& other) {
        // Защита от самоприсваивания (arr = arr)
        if (this != &other) {
            // Выделяем новую память перед удалением старой (идиома Strong Exception Guarantee)
            int* new_data = new int[other.capacity];
            for (size_t i = 0; i < other.size; ++i) {
                new_data[i] = other.data[i];
            }

            // Освобождаем старую память и обновляем поля
            delete[] data;
            data = new_data;
            capacity = other.capacity;
            size = other.size;
        }
        return *this;
    }

    // 5. Функция добавления элемента
    void add_element(int value) {
        if (size >= capacity) {
            throw std::out_of_range("Превышен лимит выделенной памяти умного массива!");
        }
        data[size] = value;
        ++size;
    }

    // 6. Функция получения элемента по индексу
    int get_element(size_t index) const {
        if (index >= size) {
            throw std::out_of_range("Индекс выходит за пределы заполненной области массива!");
        }
        return data[index];
    }

    // Дополнительный метод для получения текущего размера
    size_t get_size() const {
        return size;
    }
};

// ============================================================================
// ТЕСТИРОВАНИЕ И ПРОВЕРКА РАБОТЫ
// ============================================================================

void test_task_1() {
    std::cout << "=== Тестирование Задачи 1 (Базовый функционал RAII) ===\n";
    try {
        smart_array arr(5);
        arr.add_element(1);
        arr.add_element(4);
        arr.add_element(155);
        arr.add_element(14);
        arr.add_element(15);

        std::cout << "Элемент с индексом 1: " << arr.get_element(1) << " (ожидается: 4)\n";

        // Проверка исключения при выходе за пределы емкости
        std::cout << "Попытка добавить 6-й элемент в массив емкостью 5:\n";
        arr.add_element(999);
    } 
    catch (const std::exception& ex) {
        std::cout << "Перехвачено исключение: " << ex.what() << "\n";
    }
    std::cout << "\n";
}

void test_task_2() {
    std::cout << "=== Тестирование Задачи 2 (Копирование умных массивов) ===\n";
    try {
        smart_array arr(5);
        arr.add_element(1);
        arr.add_element(4);
        arr.add_element(155);

        smart_array new_array(2);
        new_array.add_element(44);
        new_array.add_element(34);

        std::cout << "Выполняем присваивание: arr = new_array\n";
        arr = new_array;

        std::cout << "Элементы arr после присваивания: ";
        for (size_t i = 0; i < arr.get_size(); ++i) {
            std::cout << arr.get_element(i) << " ";
        }
        std::cout << "\n";

        // Проверка независимости массивов (глубокое копирование)
        std::cout << "Проверка глубокого копирования (изменение new_array не влияет на arr)...\n";
        smart_array copy_array = arr; // Конструктор копирования
        std::cout << "Элемент 0 в скопированном массиве: " << copy_array.get_element(0) << "\n";
    }
    catch (const std::exception& ex) {
        std::cout << "Перехвачено исключение: " << ex.what() << "\n";
    }
    std::cout << "\n";
}

int main() {
    // Настройка русской локализации для корректного вывода в консоль
    std::setlocale(LC_ALL, "Russian");

    test_task_1();
    test_task_2();

    return 0;
}