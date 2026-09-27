#pragma once

#include <charconv>
#include <map>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>

/**
 * @file ini_parser.hpp
 * @brief Простой парсер INI-файлов, реализованный строго по условию
 *        курсового проекта «Парсер INI-файлов».
 *
 * Формат, который понимает парсер:
 *  - `[Имя секции]`               — заголовок секции;
 *  - `имя = значение`             — переменная (пробелы вокруг `=` любые);
 *  - `; текст`                    — комментарий, игнорируется целиком;
 *  - пустая строка                — игнорируется;
 *  - значение — либо строка, либо число (несколько значений не допускаются);
 *  - одна и та же секция может встречаться в файле несколько раз — во
 *    всех вхождениях переменные добавляются в одну и ту же секцию, а
 *    повторное присваивание переменной заменяет предыдущее значение.
 *
 * Переменные, встреченные до первого заголовка `[Секция]`, относятся к
 * неявной "глобальной" секции с именем `""` и доступны по пути `.имя`
 * (например, `get_value<int>(".version")`).
 */

namespace ini {

/**
 * @brief Единственный тип исключения, который бросает парсер.
 *
 * Покрывает все нештатные ситуации, перечисленные в задании: файл не
 * открылся, файл содержит синтаксическую ошибку (сообщение включает номер
 * строки), запрошенной переменной или секции не существует (сообщение
 * подсказывает имена существующих переменных в этой секции), а также
 * ошибку преобразования значения к запрошенному типу.
 */
class ini_error : public std::runtime_error {
public:
    explicit ini_error(const std::string& message) : std::runtime_error(message) {}
};

/**
 * @brief Загружает и разбирает один INI-файл, предоставляя типобезопасный
 *        доступ к его значениям.
 *
 * Использование ровно как в задании:
 * @code
 * ini::ini_parser parser("filename");
 * auto value = parser.get_value<int>("section.value");
 * @endcode
 */
class ini_parser {
public:
    /**
     * @brief Открывает и полностью разбирает указанный файл.
     * @param filename Путь к INI-файлу.
     * @throws ini_error если файл не удалось открыть или прочитать, а
     *         также если в файле встретилась синтаксическая ошибка
     *         (сообщение исключения содержит номер строки).
     */
    explicit ini_parser(const std::string& filename);

    /**
     * @brief Возвращает значение переменной, преобразованное к типу @p T.
     *
     * @tparam T Тип результата: любой арифметический тип (через
     *           std::from_chars) либо std::string (значение возвращается
     *           как есть, без преобразования).
     * @param path Путь вида `"секция.переменная"`. Чтобы обратиться к
     *             переменной вне любой секции, используйте путь вида
     *             `".переменная"`.
     * @return Значение переменной, преобразованное к @p T.
     *
     * @throws ini_error в одном из случаев:
     *         - путь не содержит точки;
     *         - секция с таким именем не найдена;
     *         - переменная не найдена — сообщение подскажет, какие
     *           переменные есть в этой секции (защита от опечатки);
     *         - значение переменной нельзя преобразовать к типу @p T.
     */
    template <typename T>
    T get_value(const std::string& path) const {
        const auto [section, key] = split_path(path);
        const std::string& raw = find_raw_value(section, key);
        return convert_value<T>(raw, section, key);
    }

public:
    /// Одна секция: имя переменной -> её "сырое" (ещё не преобразованное) значение.
    using section_t = std::map<std::string, std::string>;

    /**
     * @brief Возвращает все разобранные секции файла для обобщённого обхода
     *        (например, чтобы вывести весь файл целиком, не зная заранее
     *        имён секций и переменных).
     *
     * @return Отображение "имя секции -> (имя переменной -> сырое значение)".
     *         Переменные, встреченные до первого заголовка `[Секция]`,
     *         лежат в секции с пустым именем `""`.
     *
     * @note Порядок ключей — лексикографический (std::map), а не порядок
     *       следования в исходном файле: секции и переменные внутри них
     *       при выводе будут отсортированы по имени.
     */
    [[nodiscard]] const std::map<std::string, section_t>& sections() const noexcept { return sections_; }

private:
    /// Разбирает содержимое файла, заполняя sections_. Бросает ini_error
    /// с номером строки при первой синтаксической ошибке.
    void parse(const std::string& text);

    /// Убирает пробелы и табуляции по краям @p sv, не копируя данные.
    static std::string_view trim(std::string_view sv);

    /// Делит "секция.переменная" на пару (секция, переменная).
    static std::pair<std::string, std::string> split_path(const std::string& path);

    /// Ищет переменную; бросает ini_error с подсказкой, если не находит.
    const std::string& find_raw_value(const std::string& section, const std::string& key) const;

    /// Строит текст подсказки "доступные переменные: ..." для сообщения об ошибке.
    static std::string suggest_alternatives(const section_t& section);

    /// Бросает ini_error с номером строки и текстом самой строки.
    [[noreturn]] void throw_syntax_error(std::size_t line_number,
                                          std::string_view line,
                                          const std::string& reason) const;

    /// Преобразует "сырую" строку значения к типу @p T; std::string — как есть,
    /// арифметические типы — через std::from_chars.
    template <typename T>
    static T convert_value(const std::string& raw, const std::string& section, const std::string& key) {
        if constexpr (std::is_same_v<T, std::string>) {
            return raw;
        } else {
            static_assert(std::is_arithmetic_v<T>,
                          "get_value<T>: T должен быть std::string или арифметическим типом");
            T value{};
            const auto [ptr, ec] = std::from_chars(raw.data(), raw.data() + raw.size(), value);
            if (ec != std::errc{} || ptr != raw.data() + raw.size()) {
                throw ini_error("Значение переменной '" + section + "." + key + "' (\"" + raw +
                                 "\") нельзя преобразовать к запрошенному типу.");
            }
            return value;
        }
    }

    std::string filename_;                 ///< Путь к файлу — только для текстов ошибок.
    std::map<std::string, section_t> sections_; ///< Все секции файла, ключ — имя секции.
};

} // namespace ini