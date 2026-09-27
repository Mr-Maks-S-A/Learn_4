#include <ini_parser.hpp>

#include <fstream>
#include <sstream>

/**
 * @file ini_parser.cpp
 * @brief Реализация ini::ini_parser.
 */

namespace ini {

std::string_view ini_parser::trim(std::string_view sv) {
    const auto is_space = [](char c) { return c == ' ' || c == '\t' || c == '\r'; };
    std::size_t begin = 0;
    std::size_t end = sv.size();
    while (begin < end && is_space(sv[begin])) ++begin;
    while (end > begin && is_space(sv[end - 1])) --end;
    return sv.substr(begin, end - begin);
}

std::pair<std::string, std::string> ini_parser::split_path(const std::string& path) {
    const auto dot = path.find('.');
    if (dot == std::string::npos) {
        throw ini_error("Некорректный путь к значению: \"" + path +
                         "\". Ожидается формат \"секция.переменная\".");
    }
    return { path.substr(0, dot), path.substr(dot + 1) };
}

std::string ini_parser::suggest_alternatives(const section_t& section) {
    if (section.empty()) {
        return " В этой секции нет ни одной переменной.";
    }
    std::ostringstream out;
    out << " Доступные переменные в этой секции:";
    bool first = true;
    for (const auto& [name, value] : section) {
        (void)value;
        out << (first ? " " : ", ") << name;
        first = false;
    }
    out << ".";
    return out.str();
}

void ini_parser::throw_syntax_error(std::size_t line_number,
                                     std::string_view line,
                                     const std::string& reason) const {
    std::ostringstream out;
    out << "Ошибка синтаксиса в файле \"" << filename_ << "\", строка " << line_number
        << ": " << reason << " (\"" << line << "\").";
    throw ini_error(out.str());
}

const std::string& ini_parser::find_raw_value(const std::string& section, const std::string& key) const {
    const auto section_it = sections_.find(section);
    if (section_it == sections_.end()) {
        throw ini_error("Секция \"" + section + "\" не найдена в файле \"" + filename_ + "\".");
    }
    const auto key_it = section_it->second.find(key);
    if (key_it == section_it->second.end()) {
        throw ini_error("Переменная \"" + key + "\" не найдена в секции \"" + section + "\"." +
                         suggest_alternatives(section_it->second));
    }
    return key_it->second;
}

/**
 * @brief Построчный разбор INI-текста.
 *
 * Идёт по буферу без промежуточных копий (каждая строка — std::string_view
 * в исходный текст). Для каждой строки:
 *  - пустая строка или строка, начинающаяся с `;` — пропускается;
 *  - строка вида `[Имя]` — открывает (или переоткрывает) секцию;
 *    `sections_[имя]` создаёт секцию при первом обращении и возвращает ту
 *    же самую секцию при повторных `[Имя]` — благодаря этому повторяющиеся
 *    секции из задания корректно объединяются в одну;
 *  - строка вида `ключ = значение` — добавляет/перезаписывает переменную
 *    в текущей секции (текущая секция изначально — неявная, с именем "").
 *
 * @throws ini_error при первой синтаксической ошибке (см. throw_syntax_error).
 */
void ini_parser::parse(const std::string& text) {
    std::string current_section; // "" — неявная секция для строк до первого заголовка

    std::size_t pos = 0;
    std::size_t line_number = 0;

    while (pos <= text.size()) {
        ++line_number;
        const std::size_t line_end = text.find('\n', pos);
        const std::string_view raw_line = (line_end == std::string::npos)
            ? std::string_view(text).substr(pos)
            : std::string_view(text).substr(pos, line_end - pos);

        std::string_view line = trim(raw_line);

        // Строка, целиком являющаяся комментарием, пропускается без разбора.
        if (!line.empty() && line.front() == ';') {
            line = {};
        } else if (const auto semicolon = line.find(';'); semicolon != std::string_view::npos) {
            // Комментарий может идти не только отдельной строкой, но и следом
            // за значением или заголовком секции на той же строке — ровно
            // как в примере из задания ("var1=5.0 ; иногда допускается...").
            line = trim(line.substr(0, semicolon));
        }

        if (!line.empty()) {
            if (line.front() == '[') {
                if (line.size() < 2 || line.back() != ']') {
                    throw_syntax_error(line_number, raw_line,
                                        "заголовок секции начинается с '[', но не закрыт символом ']'");
                }
                current_section = std::string(trim(line.substr(1, line.size() - 2)));
                sections_.try_emplace(current_section); // секция появится в sections_, даже если пустая
            } else {
                const std::size_t eq = line.find('=');
                if (eq == std::string_view::npos) {
                    throw_syntax_error(line_number, raw_line, "отсутствует знак '=' в определении переменной");
                }

                std::string key(trim(line.substr(0, eq)));
                std::string value(trim(line.substr(eq + 1)));

                if (key.empty()) {
                    throw_syntax_error(line_number, raw_line, "имя переменной не может быть пустым");
                }

                sections_[current_section][std::move(key)] = std::move(value);
            }
        }

        if (line_end == std::string::npos) break;
        pos = line_end + 1;
    }
}

ini_parser::ini_parser(const std::string& filename) : filename_(filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        throw ini_error("Не удалось открыть файл \"" + filename + "\".");
    }

    std::ostringstream buffer;
    buffer << file.rdbuf();
    if (file.bad()) {
        throw ini_error("Ошибка чтения файла \"" + filename + "\".");
    }

    parse(buffer.str());
}

} // namespace ini
