cmake_minimum_required(VERSION 3.9.1)
project(CLient)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB_RECURSE SOURCES "src/*.cpp")


add_executable(client ${SOURCES})

# is it the right way to add all the include files 
# if I include files in src folder as "../include/FILE"?
target_include_directories(client PRIVATE ${PROJECT_SOURCE_DIR}/include)


set_target_properties(client PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Compiler options using generator expressions
target_compile_options(client PRIVATE
    $<$<CONFIG:Debug>:-fsanitize=address -Wall -Wextra -Werror -g -O1>
    $<$<CONFIG:Release>:-Wall -Wextra -O3 -DNDEBUG>
)

target_link_libraries(client PRIVATE
    $<$<CONFIG:Debug>:-fsanitize=address>
)