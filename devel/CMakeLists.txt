cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
#SET(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/libtorch/")

project(ACCPO)
set(CMAKE_CXX_STANDARD 17)

find_package(Torch REQUIRED)

set(GCC_COVERAGE_COMPILE_FLAGS "-Wall -Weffc++ -Wno-error=effc++ -pedantic \
-Wno-newline-eof -pedantic-errors \
-Wextra -Waggregate-return -Wcast-align -Wcast-qual -Wconversion -Wdisabled-optimization \
-Wfloat-equal -Wformat=2 -Wformat-nonliteral -Wformat-security -Wformat-y2k \
-Wimplicit -Wimport -Winit-self -Winline -Winvalid-pch -Wlong-long \
-Wmissing-field-initializers -Wmissing-format-attribute -Wmissing-include-dirs \
-Wmissing-noreturn -Wpacked -Wpointer-arith -Wredundant-decls -Wshadow \
-Wstack-protector -Wstrict-aliasing=2 -Wswitch-default -Wunreachable-code \
-Wunused -Wunused-parameter -Wvariadic-macros -Wwrite-strings")

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TORCH_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")

INCLUDE_DIRECTORIES("${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/kraken/")
LINK_DIRECTORIES("/usr/lib/")
LINK_DIRECTORIES("/usr/local/lib/")
LINK_DIRECTORIES("/opt/local/lib")

file(GLOB_RECURSE SOURCES "source/*.cpp")
file(GLOB_RECURSE HEADERS "source/*.h")
add_executable(ACCPO ${SOURCES} ${HEADERS})

#target_link_libraries(ACCPO "${TORCH_LIBRARIES}")
target_link_libraries(ACCPO m ssl crypto)
target_link_libraries(ACCPO "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/kraken/libkraken.a")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set_property(TARGET ACCPO PROPERTY CXX_STANDARD 17)
target_compile_options(ACCPO PRIVATE -std=c++17 -o2 -ansi -fsigned-char -D_FORTIFY_SOURCE=2)
