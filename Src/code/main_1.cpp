#include <iostream>
#include <vector>
#include <memory>
#include <clocale>

class tridiagonal_matrix
{
public:
    std::vector<double> m_down;
    std::vector<double> m_upper;
    std::vector<double> m_middle;

    tridiagonal_matrix(
        const std::vector<double>& down,
        const std::vector<double>& upper,
        const std::vector<double>& middle) :
        m_down{ down }, m_upper{ upper }, m_middle{ middle }
    {};

    ~tridiagonal_matrix() 
    { 
        std::cout << "destructor called (tridiagonal_matrix at " << this << ")\n"; 
    }

    // Реализация метода клонирования
    // Квалификатор const указывает, что метод не меняет текущую матрицу.
    std::unique_ptr<tridiagonal_matrix> clone() const
    {
        // Создаем и возвращаем новый экземпляр через std::make_unique,
        // передавая копии векторов текущего объекта (m_down, m_upper, m_middle).
        return std::make_unique<tridiagonal_matrix>(m_down, m_upper, m_middle);
    }
};

int main()
{
    std::setlocale(LC_ALL, "Russian");
    auto down = std::vector<double>{ -4.0, 5.0 };
    auto upper = std::vector<double>{ 14.0, 8.0 };
    auto middle = std::vector<double>{ 3.0, 1.0, 7.0 };

    // Создаем матрицу в динамической памяти через unique_ptr
    auto matrix = std::make_unique<tridiagonal_matrix>(
        down,
        upper,
        middle
    );

    std::cout << "Оригинальная матрица создана по адресу: " << matrix.get() << "\n";

    // Клонируем матрицу
    auto matrix_clone = matrix->clone();

    std::cout << "Клон матрицы создан по адресу: " << matrix_clone.get() << "\n";

    // Проверка независимости данных: меняем клон, оригинал не меняется
    matrix_clone->m_middle[0] = 999.0;
    std::cout << "Элемент клона: " << matrix_clone->m_middle[0] << "\n";
    std::cout << "Элемент оригинала: " << matrix->m_middle[0] << "\n\n";

    std::cout << "Завершение main(), очистка памяти:\n";
    return 0;
}
