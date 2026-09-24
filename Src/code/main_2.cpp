#include <iostream>
#include <vector>
#include <memory>
#include <clocale>


class node
{
public:
    int m_value;
    
    // Используем weak_ptr вместо shared_ptr.
    // weak_ptr ссылается на объект, но НЕ увеличивает счетчик сильного владения (strong reference count).
    std::weak_ptr<node> parent;

    node(int value) : m_value{ value } 
    {
        std::cout << "node(" << m_value << ") constructor called\n";
    };

    ~node() 
    { 
        std::cout << "destructor called for node(" << m_value << ")\n"; 
    }
};

int main()
{
    std::setlocale(LC_ALL, "Russian");
    {
        auto node1 = std::make_shared<node>(1);
        auto node2 = std::make_shared<node>(2);

        // Устанавливаем перекрестные ссылки
        node1->parent = node2; // weak_ptr принимает shared_ptr без увеличения счетчика
        node2->parent = node1;

        std::cout << "Счетчик ссылок node1: " << node1.use_count() << "\n"; // Выведет 1
        std::cout << "Счетчик ссылок node2: " << node2.use_count() << "\n"; // Выведет 1

        // Проверка доступа к parent через lock():
        if (auto parent_ptr = node1->parent.lock()) {
            std::cout << "Родитель node1 имеет значение: " << parent_ptr->m_value << "\n";
        }
    } // Выход из области видимости: node1 и node2 уничтожаются, выводится 2 деструктора.

    std::cout << "Завершение main()\n";
    return 0;
}
