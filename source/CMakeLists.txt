add_executable(HW1 main.cpp)
target_link_libraries(HW1 ${CONAN_LIBS})

file(GLOB_RECURSE INCLUDE *.hpp)
file(GLOB_RECURSE SOURCE *.cpp)

target_include_directories(HW1
    PUBLIC
        .
    )

target_sources(HW1
    PUBLIC
        ${INCLUDE}
    PRIVATE
        ${SOURCE}
)