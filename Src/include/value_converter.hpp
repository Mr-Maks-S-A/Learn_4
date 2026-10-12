#pragma once

#include <charconv>
#include <optional>
#include <string>
#include <type_traits>

/**
 * @file value_converter.hpp
 * @brief Расширяемая таблица преобразований «строка из INI -> C++ тип».
 *
 * Правила разбора каждого типа живут в отдельной (частичной) специализации
 * шаблона ini::value_converter<T>. Чтобы поддержать новый тип или изменить
 * правило для существующего (например, формат вещественных чисел), достаточно
 * добавить/поправить ОДНУ специализацию — ini_parser::get_value не меняется.
 *
 * Контракт специализации: статический метод
 *     static std::optional<T> convert(const std::string& raw);
 * возвращает std::nullopt, если строка не является корректным значением типа T.
 * Исключение (с именем секции и переменной) бросает уже парсер.
 */

namespace ini {

namespace detail {
template <typename>
inline constexpr bool always_false_v = false;
} // namespace detail

/// Основной шаблон: тип без зарегистрированного правила — ошибка компиляции
/// с понятным сообщением (в точке инстанцирования, а не в теле функции).
template <typename T, typename Enable = void>
struct value_converter {
    static_assert(detail::always_false_v<T>,
                  "ini::value_converter<T>: для этого типа нет правила преобразования. "
                  "Добавьте специализацию value_converter.");
};

/// std::string — значение как есть.
template <>
struct value_converter<std::string> {
    static std::optional<std::string> convert(const std::string& raw) { return raw; }
};

/// bool: true/false/yes/no/on/off/1/0 (регистр не важен).
template <>
struct value_converter<bool> {
    static std::optional<bool> convert(const std::string& raw) {
        std::string s;
        for (char c : raw) s += static_cast<char>(c >= 'A' && c <= 'Z' ? c - 'A' + 'a' : c);
        if (s == "true" || s == "yes" || s == "on" || s == "1") return true;
        if (s == "false" || s == "no" || s == "off" || s == "0") return false;
        return std::nullopt;
    }
};

/// Целые числа: строго целиком строка, дробная часть — ошибка (не обрезается).
template <typename T>
struct value_converter<T, std::enable_if_t<std::is_integral_v<T> && !std::is_same_v<T, bool>>> {
    static std::optional<T> convert(const std::string& raw) {
        T value{};
        const char* end = raw.data() + raw.size();
        const auto [ptr, ec] = std::from_chars(raw.data(), end, value);
        if (ec != std::errc{} || ptr != end) return std::nullopt;
        return value;
    }
};

/// Вещественные числа: разделитель дробной части — и '.', и ','.
/// Допускается не более одного разделителя; хвост после числа — ошибка.
template <typename T>
struct value_converter<T, std::enable_if_t<std::is_floating_point_v<T>>> {
    static std::optional<T> convert(const std::string& raw) {
        std::string normalized = raw;
        std::size_t separators = 0;
        for (char& c : normalized) {
            if (c == ',') c = '.';
            if (c == '.') ++separators;
        }
        if (separators > 1) return std::nullopt;

        T value{};
        const char* end = normalized.data() + normalized.size();
        const auto [ptr, ec] = std::from_chars(normalized.data(), end, value);
        if (ec != std::errc{} || ptr != end) return std::nullopt;
        return value;
    }
};

} // namespace ini
