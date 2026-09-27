/**
 * @file main.cpp
 * @brief Пример использования ini::ini_parser
 *
 * Путь до INI-файла передаётся первым аргументом командной строки.
 *  - Если аргумент не передан — берётся "Source/config.ini" и выводятся
 *    несколько конкретных, заранее известных значений (демонстрация
 *    типизированного доступа get_value<T>).
 *  - Если аргумент передан — файл разбирается и выводится целиком, без
 *    привязки к конкретным именам секций/переменных (демонстрация
 *    обобщённого обхода через sections()).
 *
 * Запуск:
 *     ./pars_app                     # возьмёт Source/config.ini, разбор конкретных полей
 *     ./pars_app path/to/other.ini   # возьмёт указанный файл, выведет его целиком
 */

#include <ini_parser.hpp>

#include <clocale>
#include <iostream>

namespace {

/// Выводит все секции и переменные файла как есть, без обращения к
/// конкретным именам — то, что нужно, когда структура файла заранее
/// неизвестна.
void dump_whole_file(const ini::ini_parser& parser) {
    for (const auto& [section_name, variables] : parser.sections()) {
        if (!section_name.empty()) {
            std::cout << "[" << section_name << "]\n";
        }
        for (const auto& [key, value] : variables) {
            std::cout << key << " = " << value << "\n";
        }
        std::cout << "\n";
    }
}

/// Демонстрация типизированного доступа к заранее известным полям
/// config.ini по умолчанию — включая бонусное требование про подсказку
/// при опечатке в имени переменной.
void run_known_fields_demo(const ini::ini_parser& parser) {
    std::cout << "[Section1] var1 = " << parser.get_value<double>("Section1.var1") << "\n";
    std::cout << "[Section1] var2 = " << parser.get_value<std::string>("Section1.var2") << "\n";
    std::cout << "[Section1] var3 = " << parser.get_value<std::string>("Section1.var3") << "\n";
    std::cout << "[Section2] var1 = " << parser.get_value<int>("Section2.var1") << "\n";
    std::cout << "[Section4] Mode = " << parser.get_value<std::string>("Section4.Mode") << "\n";
    std::cout << "[Section4] Level = " << parser.get_value<int>("Section4.Level") << "\n";

    try {
        parser.get_value<std::string>("Section4.mode");
    } catch (const ini::ini_error& e) {
        std::cout << "\nОжидаемая ошибка при опечатке в имени переменной:\n  " << e.what() << "\n";
    }
}

} // namespace

int main(int argc, char* argv[]) {
#ifdef _WIN32
    std::setlocale(LC_ALL, "Russian");
#else
    std::setlocale(LC_ALL, "ru_RU.UTF-8");
#endif

    if (argc > 2) {
        std::cerr << "Использование: " << argv[0] << " [путь_до_ini_файла]\n";
        return 2;
    }

    const bool custom_path = argc > 1;
    const std::string path = custom_path ? argv[1] : "Source/config.ini";

    try {
        const ini::ini_parser parser(path);

        if (custom_path) {
            dump_whole_file(parser);
        } else {
            run_known_fields_demo(parser);
        }
    } catch (const ini::ini_error& e) {
        // Сюда попадут: файл не открылся, синтаксическая ошибка (с номером
        // строки) или обращение к несуществующей секции/переменной.
        std::cerr << "Ошибка: " << e.what() << "\n";
        return 1;
    }

    return 0;
}