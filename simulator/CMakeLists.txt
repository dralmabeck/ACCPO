cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

project(ACCPO)
set(CMAKE_CXX_STANDARD 17)

set(GCC_COVERAGE_COMPILE_FLAGS "-Wall -Wno-error=effc++ -pedantic -pedantic-errors -Wextra -Werror")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TORCH_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")

INCLUDE_DIRECTORIES("${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/kraken/")
LINK_DIRECTORIES("${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/kraken/")
LINK_DIRECTORIES("/usr/lib/")
LINK_DIRECTORIES("/usr/local/lib/")
LINK_DIRECTORIES("/opt/local/lib")

file(GLOB_RECURSE SOURCES "source/*.cpp")
file(GLOB_RECURSE HEADERS "source/*.h")
add_executable(ACCPO ${SOURCES} ${HEADERS})

target_link_libraries(ACCPO pthread kraken m ssl crypto)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set_property(TARGET ACCPO PROPERTY CXX_STANDARD 17)
target_compile_options(ACCPO PRIVATE -std=c++17 -o2 -ansi -fsigned-char -D_FORTIFY_SOURCE=2)
