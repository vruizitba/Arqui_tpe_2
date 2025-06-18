cmake_minimum_required(VERSION 3.16)
project(Arqui_TPE C)

set(CMAKE_C_FLAGS "-Wall -pedantic -std=c99 -lm -g -fsanitize=address")

# Buscar automáticamente todos los archivos .c y .asm recursivamente
file(GLOB_RECURSE KERNEL_SRC
        "g25-64046-65531/Kernel/*.c"
        "g25-64046-65531/Kernel/*.asm"
        "g25-64046-65531/Kernel/*.h"
)

file(GLOB_RECURSE USERLAND_SRC
        "g25-64046-65531/Userland/*.c"
        "g25-64046-65531/Userland/*.asm"
        "g25-64046-65531/Userland/*.h"
)

add_executable(Arqui_TPE
        main.c
        ${KERNEL_SRC}
        ${USERLAND_SRC}
)

# Configurar directorios de inclusión
target_include_directories(Arqui_TPE PRIVATE
        g25-64046-65531/Kernel/include
        g25-64046-65531/Userland/SampleCodeModule/include
)