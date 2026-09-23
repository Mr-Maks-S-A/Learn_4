cmake_minimum_required(VERSION 3.16)
project(ExternalLibrary CXX)

# ------------------------------------------------------------------------------
# Опции внешних библиотек (отключаем лишнее, чтобы сократить время сборки)
# ------------------------------------------------------------------------------

# Catch2 (v3)
set(CATCH_BUILD_TESTING OFF CACHE BOOL "" FORCE)
set(CATCH_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(CATCH_INSTALL_DOCS OFF CACHE BOOL "" FORCE)
set(CATCH_INSTALL_HELPERS OFF CACHE BOOL "" FORCE)
set(BUILD_TESTING OFF CACHE BOOL "" FORCE)

# ------------------------------------------------------------------------------
# Добавление поддиректорий
# ------------------------------------------------------------------------------
add_subdirectory(Catch2)

# ------------------------------------------------------------------------------
# Создание интерфейсной библиотеки-обертки для удобного подключения в проект
# ------------------------------------------------------------------------------
add_library(ExternalLibrary INTERFACE)

target_link_libraries(ExternalLibrary INTERFACE
    Catch2::Catch2WithMain # Включает сам Catch2 + готовый main() для тестов
)
