cmake_minimum_required(VERSION 3.16)

project(
    pars_app
    VERSION 1.0
    LANGUAGES CXX
)

# ============================================================
# Настройки проекта
# ============================================================

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Собирать ли тесты
option(BUILD_TESTS "Build unit tests" OFF)

# ============================================================
# Директории вывода
# ============================================================

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")


# ============================================================
# Основное приложение
# ============================================================

set(APP_SOURCES
    "${CMAKE_SOURCE_DIR}/Src/code/main.cpp"
    "${CMAKE_SOURCE_DIR}/Src/code/ini_parser.cpp"
)

set(APP_HEADERS
    "${CMAKE_SOURCE_DIR}/Src/include/ini_parser.hpp"
)

add_executable(
    pars_app
    ${APP_SOURCES}
    ${APP_HEADERS}
)

target_include_directories(
    pars_app
    PRIVATE
        "${CMAKE_SOURCE_DIR}/Src/include"
)

# ============================================================
# Копирование ресурсов
# ============================================================

add_custom_command(
    TARGET pars_app
    POST_BUILD

    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/Source"
        "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Source"

    COMMENT "Копирование папки Source..."
)

# ============================================================
# Тесты
# ============================================================

if(BUILD_TESTS)
    # ============================================================
    # Внешние зависимости
    # ============================================================

    add_subdirectory("${CMAKE_SOURCE_DIR}/ExternalLibrary")


    message(STATUS "Building tests: ON")

    enable_testing()

    include(CTest)

    set(TEST_SOURCES
        "${CMAKE_SOURCE_DIR}/Src/code/ini_parser.cpp"
        "${CMAKE_SOURCE_DIR}/Tests/test_ini_parser.cpp"
    )

    add_executable(
        pars_app_tests
        ${TEST_SOURCES}
    )

    target_include_directories(
        pars_app_tests
        PRIVATE
            "${CMAKE_SOURCE_DIR}/Src/include"
    )

    target_link_libraries(
        pars_app_tests
        PRIVATE
            doctest::doctest
    )

    add_test(
        NAME ini_parser_tests
        COMMAND pars_app_tests
    )

else()

    message(STATUS "Building tests: OFF")

endif()
