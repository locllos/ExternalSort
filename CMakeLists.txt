cmake_minimum_required (VERSION 3.15)

project(HW1 CXX)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(fmt REQUIRED)

add_compile_options("-g3")
add_compile_options("-Wall")
add_compile_options("-Wextra")
add_compile_options("-Wpedantic")
add_compile_options("-fsanitize=address")
add_link_options("-fsanitize=address")

add_subdirectory(source)

target_link_libraries(HW1 ${CONAN_LIBS})
target_link_libraries(${PROJECT_NAME} fmt::fmt)