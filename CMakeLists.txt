# Минимально требуемая версия CMake
cmake_minimum_required(VERSION 3.16)

project(app VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Выходные директории для бинарников и библиотек
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)


# Создаем интерфейсную библиотеку с заголовочными файлами вашего проекта
add_library(APP_utils INTERFACE)
target_include_directories(APP_utils INTERFACE "${CMAKE_SOURCE_DIR}/Src/include")

# Создание основного исполняемого файла
add_executable(app_1 "${CMAKE_SOURCE_DIR}/Src/code/main.cpp")
target_link_libraries(app_1 PRIVATE APP_utils)

