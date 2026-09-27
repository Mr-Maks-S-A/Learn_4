/**
 * @file test_ini_parser.cpp
 * @brief Модульные тесты для ini::ini_parser на фреймворке doctest.
 *
 * Сборка (doctest.h должен быть доступен в include-путях, например через
 * vcpkg/Conan, либо просто положите однофайловый doctest.h рядом):
 *
 *     g++ -std=c++17 -I. -o test_ini_parser test_ini_parser.cpp ini_parser.cpp
 *     ./test_ini_parser
 *
 * Каждый тест создаёт свой временный .ini-файл (см. temp_ini_file ниже),
 * потому что ini_parser, как и требует задание, работает с файлом по имени,
 * а не с буфером в памяти — так тесты проверяют ровно тот путь, которым
 * пользуется реальный код.
 */

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include <ini_parser.hpp>

#include <cstdio>
#include <filesystem>
#include <fstream>
#include <string>

namespace {

/**
 * @brief RAII-обёртка над временным .ini-файлом: создаёт файл с заданным
 *        содержимым в конструкторе и удаляет его в деструкторе.
 */
class temp_ini_file {
public:
    explicit temp_ini_file(const std::string& content) {
        const auto dir = std::filesystem::temp_directory_path();
        path_ = (dir / ("ini_parser_test_" + std::to_string(counter_++) + ".ini")).string();
        std::ofstream out(path_, std::ios::binary);
        out << content;
    }

    ~temp_ini_file() {
        std::error_code ec;
        std::filesystem::remove(path_, ec);
    }

    temp_ini_file(const temp_ini_file&) = delete;
    temp_ini_file& operator=(const temp_ini_file&) = delete;

    const std::string& path() const { return path_; }

private:
    std::string path_;
    static inline int counter_ = 0;
};

} // namespace

TEST_CASE("читает числовые и строковые значения") {
    temp_ini_file file(
        "[Section1]\n"
        "var1 = 5\n"
        "var2 = какая-то строка\n");

    ini::ini_parser parser(file.path());

    CHECK(parser.get_value<int>("Section1.var1") == 5);
    CHECK(parser.get_value<std::string>("Section1.var2") == "какая-то строка");
}

TEST_CASE("понимает вещественные числа") {
    temp_ini_file file("[Section1]\nvar1 = 5.5\n");
    ini::ini_parser parser(file.path());

    CHECK(parser.get_value<double>("Section1.var1") == doctest::Approx(5.5));
}

TEST_CASE("пробелы вокруг '=' могут быть произвольными") {
    temp_ini_file file("[S]\nkey=1\nkey2   =   2\nkey3=3\n");
    ini::ini_parser parser(file.path());

    CHECK(parser.get_value<int>("S.key") == 1);
    CHECK(parser.get_value<int>("S.key2") == 2);
    CHECK(parser.get_value<int>("S.key3") == 3);
}

TEST_CASE("комментарии и пустые строки игнорируются") {
    temp_ini_file file(
        "; это комментарий\n"
        "\n"
        "[Section1]\n"
        "\n"
        "; ещё комментарий\n"
        "var1 = 1\n");

    ini::ini_parser parser(file.path());
    CHECK(parser.get_value<int>("Section1.var1") == 1);
}

TEST_CASE("комментарий может идти после значения на той же строке") {
    temp_ini_file file(
        "[Section1]; комментарий о разделе\n"
        "var1 = 5.0 ; иногда допускается комментарий к отдельному параметру\n");

    ini::ini_parser parser(file.path());
    CHECK(parser.get_value<double>("Section1.var1") == doctest::Approx(5.0));
}

TEST_CASE("секция без переменных парсится, но не содержит значений") {
    temp_ini_file file("[Section3]\n[Section4]\nMode = release\n");
    ini::ini_parser parser(file.path());

    CHECK(parser.get_value<std::string>("Section4.Mode") == "release");
    CHECK_THROWS_AS(parser.get_value<std::string>("Section3.anything"), ini::ini_error);
}

TEST_CASE("повторяющиеся секции объединяются, а повторное присваивание переписывает значение") {
    temp_ini_file file(
        "[Section1]\n"
        "var1 = 5.0\n"
        "var2 = какая-то строка\n"
        "[Section1]\n"
        "var3 = значение\n"
        "var1 = 1.0\n");

    ini::ini_parser parser(file.path());

    CHECK(parser.get_value<double>("Section1.var1") == doctest::Approx(1.0)); // переприсвоено
    CHECK(parser.get_value<std::string>("Section1.var2") == "какая-то строка"); // осталось из первого вхождения
    CHECK(parser.get_value<std::string>("Section1.var3") == "значение"); // добавлено во втором вхождении
}

TEST_CASE("переменные до первого заголовка секции относятся к неявной глобальной секции") {
    temp_ini_file file("version = 3\n[Section1]\nvar1 = 1\n");
    ini::ini_parser parser(file.path());

    CHECK(parser.get_value<int>(".version") == 3);
    CHECK(parser.get_value<int>("Section1.var1") == 1);
}

TEST_CASE("несуществующая переменная бросает исключение с подсказкой похожих имён") {
    temp_ini_file file("[Section4]\nMode = release\nLevel = 3\n");
    ini::ini_parser parser(file.path());

    try {
        parser.get_value<std::string>("Section4.mode"); // опечатка: mode вместо Mode
        FAIL("ожидалось исключение ini::ini_error");
    } catch (const ini::ini_error& e) {
        const std::string message = e.what();
        CHECK(message.find("Mode") != std::string::npos);
        CHECK(message.find("Level") != std::string::npos);
    }
}

TEST_CASE("несуществующая секция бросает исключение") {
    temp_ini_file file("[Section1]\nvar1 = 1\n");
    ini::ini_parser parser(file.path());

    CHECK_THROWS_AS(parser.get_value<int>("NoSuchSection.var1"), ini::ini_error);
}

TEST_CASE("путь без точки некорректен") {
    temp_ini_file file("[Section1]\nvar1 = 1\n");
    ini::ini_parser parser(file.path());

    CHECK_THROWS_AS(parser.get_value<int>("Section1var1"), ini::ini_error);
}

TEST_CASE("значение, которое нельзя преобразовать к запрошенному типу, бросает исключение") {
    temp_ini_file file("[Section1]\nvar2 = какая-то строка\n");
    ini::ini_parser parser(file.path());

    CHECK_THROWS_AS(parser.get_value<int>("Section1.var2"), ini::ini_error);
}

TEST_CASE("незакрытый заголовок секции — синтаксическая ошибка с номером строки") {
    // Ошибка находится на 3-й строке файла.
    temp_ini_file file("[Section1]\nvar1 = 1\n[Section2\n");

    try {
        ini::ini_parser parser(file.path());
        FAIL("ожидалось исключение ini::ini_error");
    } catch (const ini::ini_error& e) {
        const std::string message = e.what();
        CHECK(message.find("строка 3") != std::string::npos);
    }
}

TEST_CASE("строка без знака '=' — синтаксическая ошибка") {
    temp_ini_file file("[Section1]\nvar1\n");
    CHECK_THROWS_AS(ini::ini_parser(file.path()), ini::ini_error);
}

TEST_CASE("пустое имя переменной — синтаксическая ошибка") {
    temp_ini_file file("[Section1]\n= 1\n");
    CHECK_THROWS_AS(ini::ini_parser(file.path()), ini::ini_error);
}

TEST_CASE("несуществующий файл бросает исключение") {
    CHECK_THROWS_AS(ini::ini_parser("this_file_does_not_exist.ini"), ini::ini_error);
}
