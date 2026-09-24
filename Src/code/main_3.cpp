#include <iostream>
#include <utility> 
#include <clocale>

template <typename T>
class my_unique_ptr
{
private:
    T* m_ptr{ nullptr }; // Внутренний сырой указатель

public:
    // 1. Конструкторы
    constexpr my_unique_ptr() noexcept : m_ptr(nullptr) {}

    explicit my_unique_ptr(T* ptr) noexcept : m_ptr(ptr) {}

    // 2. Деструктор
    ~my_unique_ptr()
    {
        delete m_ptr; // Освобождаем память (delete nullptr безопасен в C++)
    }

    // 3. Запрет копирования (Семантика эксклюзивного владения)
    my_unique_ptr(const my_unique_ptr&) = delete;
    my_unique_ptr& operator=(const my_unique_ptr&) = delete;

    // 4. Семантика перемещения (Move semantic)
    // Конструктор перемещения
    my_unique_ptr(my_unique_ptr&& other) noexcept : m_ptr(other.m_ptr)
    {
        other.m_ptr = nullptr; // Забираем владение ресурсом
    }

    // Оператор присваивания перемещением
    my_unique_ptr& operator=(my_unique_ptr&& other) noexcept
    {
        if (this != &other)
        {
            delete m_ptr;        // Освобождаем текущий ресурс
            m_ptr = other.m_ptr; // Перехватываем ресурс у other
            other.m_ptr = nullptr;
        }
        return *this;
    }

    // 5. Перегрузка операторов доступа
    T& operator*() const
    {
        return *m_ptr;
    }

    T* operator->() const noexcept
    {
        return m_ptr;
    }

    // Проверка на валидность указателя в условных конструкциях (if (ptr) ...)
    explicit operator bool() const noexcept
    {
        return m_ptr != nullptr;
    }

    // 6. Вспомогательные методы управления ресурсом
    
    // Освобождает владение указателем и возвращает его без удаления
    T* release() noexcept
    {
        T* temp = m_ptr;
        m_ptr = nullptr;
        return temp;
    }

    // Заменяет текущий объект новым
    void reset(T* ptr = nullptr) noexcept
    {
        T* old_ptr = m_ptr;
        m_ptr = ptr;
        delete old_ptr;
    }

    // Получение сырого указателя
    T* get() const noexcept
    {
        return m_ptr;
    }
};

// Вспомогательная функция, аналогичная std::make_unique (C++14)
template <typename T, typename... Args>
my_unique_ptr<T> make_my_unique(Args&&... args)
{
    return my_unique_ptr<T>(new T(std::forward<Args>(args)...));
}

// Тестовая структура для демонстрации
struct Item
{
    int id;
    Item(int val) : id(val) { std::cout << "Item (" << id << ") создан\n"; }
    ~Item() { std::cout << "Item (" << id << ") уничтожен\n"; }
    void show() const { std::cout << "ID элемента: " << id << "\n"; }
};

int main()
{
    std::setlocale(LC_ALL, "Russian");
    std::cout << "--- 1. Создание и доступ ---\n";
    my_unique_ptr<Item> p1(new Item(100));
    p1->show();
    std::cout << "Значение через *: " << (*p1).id << "\n";

    std::cout << "\n--- 2. Перемещение владения ---\n";
    // my_unique_ptr<Item> p2 = p1; // Ошибка компиляции! Копирование запрещено.
    my_unique_ptr<Item> p2 = std::move(p1); // Перемещение разрешено

    if (!p1) {
        std::cout << "p1 теперь пуст (nullptr)\n";
    }
    if (p2) {
        std::cout << "p2 успешно принял владение: ";
        p2->show();
    }

    std::cout << "\n--- 3. Тест метода release() ---\n";
    Item* raw_item = p2.release(); // p2 теряет право на удаление
    std::cout << "p2 освободил ресурс. Вызываем метод напрямую через сырой указатель:\n";
    raw_item->show();
    delete raw_item; // Память очищаем вручную, так как был вызван release()

    std::cout << "\n--- 4. Использование аналога make_unique ---\n";
    auto p3 = make_my_unique<Item>(200);
    p3->show();

    std::cout << "\n--- Завершение работы программы ---\n";
    return 0;
}
