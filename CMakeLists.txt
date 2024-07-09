cmake_minimum_required(VERSION 3.28)
project(compiler C)

set(CMAKE_C_STANDARD 11)

include_directories(include)

add_executable(compiler
        include/buffer.h
        include/lexer.h
        include/parser.h
        include/symbols.h
        include/syntax_analysis.h
        include/utils.h
        src/buffer.c
        src/lexer.c
        src/main.c
        src/symbols.c
        src/syntax_analysis.c
        src/utils.c)
