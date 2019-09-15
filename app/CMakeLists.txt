
cmake_minimum_required(VERSION 3.10)
project(embeddedhello C ASM)
#enable_language(ASM${ASSEMBLY_DIALECT})

include(cmake/cross_compile.cmake)

set(APP_SRCS
    src/startup.S
    src/hello.c
    src/uart.c
)

set(CMAKE_EXE_LINKER_FLAGS "-T ${CMAKE_SOURCE_DIR}/link/app.lds -nostdlib")

add_executable(${PROJECT_NAME} ${APP_SRCS})
