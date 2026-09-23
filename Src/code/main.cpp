#include <catch2/catch_test_macros.hpp>
#include <stdexcept>
#include <iostream>

// --- Предоставленный класс двусвязного списка ---
struct ListNode
{
public:
    ListNode(int value, ListNode* prev = nullptr, ListNode* next = nullptr)
        : value(value), prev(prev), next(next)
    {
        if (prev != nullptr) prev->next = this;
        if (next != nullptr) next->prev = this;
    }
public:
    int value;
    ListNode* prev;
    ListNode* next;
};

class List
{
public:
    List()
        : m_head(new ListNode(static_cast<int>(0))), m_size(0),
        m_tail(new ListNode(0, m_head))
    {       
    }

    virtual ~List()
    {
        Clear();
        delete m_head;
        delete m_tail;
    }

    bool Empty() { return m_size == 0; }

    unsigned long Size() { return m_size; }

    void PushFront(int value)
    {
        new ListNode(value, m_head, m_head->next);
        ++m_size;
    }

    void PushBack(int value)
    {
        new ListNode(value, m_tail->prev, m_tail);
        ++m_size;
    }

    int PopFront()
    {
        if (Empty()) throw std::runtime_error("list is empty");
        auto node = extractPrev(m_head->next->next);
        int ret = node->value;
        delete node;
        return ret;
    }

    int PopBack()
    {
        if (Empty()) throw std::runtime_error("list is empty");
        auto node = extractPrev(m_tail);
        int ret = node->value;
        delete node;
        return ret;
    }

    void Clear()
    {
        auto current = m_head->next;
        while (current != m_tail)
        {
            current = current->next;
            delete extractPrev(current);
        }
    }

private:
    ListNode* extractPrev(ListNode* node)
    {
        auto target = node->prev;
        target->prev->next = target->next;
        target->next->prev = target->prev;
        --m_size;
        return target;
    }

private:
    ListNode* m_head;
    ListNode* m_tail;
    unsigned long m_size;
};

// ============================================================================
// ЗАДАЧА 1. Проверка базовых функций двусвязного списка (Empty, Size, Clear)
// ============================================================================

TEST_CASE("Задача 1: Базовые функции списка (Empty, Size, Clear)", "[List][Basic]")
{
    List list;

    SECTION("Свежесозданный список является пустым")
    {
        UNSCOPED_INFO("-> Проверяем Empty() и Size() для нового списка");
        std::cout << "[LOG] Проверка пустой структуры..." << std::endl;

        CHECK(list.Empty() == true);
        CHECK(list.Size() == 0);
    }

    SECTION("Добавление элементов изменяет Size и Empty")
    {
        std::cout << "[LOG] Добавляем элементы 10, 20 (Back) и 30 (Front)..." << std::endl;
        
        list.PushBack(10);
        CHECK(list.Empty() == false);
        CHECK(list.Size() == 1);

        list.PushBack(20);
        list.PushFront(30);
        
        std::cout << "[LOG] Текущий размер списка: " << list.Size() << std::endl;
        CHECK(list.Size() == 3);
        CHECK(list.Empty() == false);
    }

    SECTION("Очистка списка с помощью Clear()")
    {
        std::cout << "[LOG] Наполняем список перед очисткой..." << std::endl;
        list.PushBack(1);
        list.PushBack(2);
        list.PushBack(3);

        REQUIRE(list.Size() == 3);

        std::cout << "[LOG] Вызов Clear()..." << std::endl;
        list.Clear();

        CHECK(list.Size() == 0);
        CHECK(list.Empty() == true);
    }

    SECTION("Повторный вызов Clear() на пустом списке безопасен")
    {
        std::cout << "[LOG] Повторный Clear() на пустом списке..." << std::endl;
        list.Clear();
        CHECK(list.Size() == 0);
        CHECK(list.Empty() == true);
    }
}

// ============================================================================
// ЗАДАЧА 2. Проверка функций PushBack, PushFront, PopBack, PopFront
// ============================================================================

TEST_CASE("Задача 2: Добавление и получение элементов", "[List][Operations]")
{
    List list;

    SECTION("Проверка PushFront и PushBack")
    {
        std::cout << "[LOG] PushFront(10) -> PushBack(20) -> PushFront(5)" << std::endl;
        list.PushFront(10); 
        list.PushBack(20);  
        list.PushFront(5);   

        CHECK(list.Size() == 3);

        std::cout << "[LOG] Извлекаем элементы..." << std::endl;
        CHECK(list.PopFront() == 5);  
        CHECK(list.PopBack() == 20);  
        CHECK(list.PopFront() == 10); 

        CHECK(list.Empty() == true);
    }

    SECTION("Вызов PopFront на пустом списке вызывает исключение std::runtime_error")
    {
        std::cout << "[LOG] Проверка исключения для PopFront на пустом списке..." << std::endl;
        CHECK_THROWS_AS(list.PopFront(), std::runtime_error);
    }

    SECTION("Вызов PopBack на пустом списке вызывает исключение std::runtime_error")
    {
        std::cout << "[LOG] Проверка исключения для PopBack на пустом списке..." << std::endl;
        CHECK_THROWS_AS(list.PopBack(), std::runtime_error);
    }

    SECTION("Сложный сценарий (комплексное взаимодействие операций)")
    {
        std::cout << "[LOG] 1. Наполнение списка 5 элементами..." << std::endl;
        for (int i = 1; i <= 5; ++i)
        {
            list.PushBack(i); 
        }
        CHECK(list.Size() == 5);

        std::cout << "[LOG] 2. PopFront() и PopBack()..." << std::endl;
        CHECK(list.PopFront() == 1); 
        CHECK(list.PopBack() == 5);  
        CHECK(list.Size() == 3);

        std::cout << "[LOG] 3. Дополнительные PushFront(100) и PushBack(200)..." << std::endl;
        list.PushFront(100); 
        list.PushBack(200);  
        CHECK(list.Size() == 5);

        std::cout << "[LOG] 4. Очистка списка и повторные тесты..." << std::endl;
        list.Clear();
        CHECK(list.Empty() == true);
        CHECK_THROWS_AS(list.PopFront(), std::runtime_error);
        CHECK_THROWS_AS(list.PopBack(), std::runtime_error);

        std::cout << "[LOG] 5. Добавление и удаление после Clear()..." << std::endl;
        list.PushBack(42);
        CHECK(list.Empty() == false);
        CHECK(list.Size() == 1);
        CHECK(list.PopFront() == 42);
        CHECK(list.Empty() == true);
    }
}