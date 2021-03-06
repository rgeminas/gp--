cmake_minimum_required (VERSION 2.8)
set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")

find_package (BISON2 REQUIRED)

# MSVC doesn't have c99 support, so we compile the project as if it were C++.
if (MSVC)
    project (gp-- C CXX)
    set (CMAKE_CXX_FLAGS CMAKE_C_FLAGS)
    set (CMAKE_CXX_FLAGS_DEBUG CMAKE_C_FLAGS_DEBUG)
else ()
    #set (CC "~/.vim/bin/cc_args.py gcc")
    project (gp-- C)
    set (CMAKE_C_FLAGS "-std=c99")
    set (CMAKE_C_FLAGS_DEBUG "-g")
endif ()

set (src src)

set(CMAKE_BUILD_TYPE Debug CACHE FILEPATH "Build type.")

include_directories (${src})

message (STATUS ${CMAKE_SOURCE_DIR})

BISON_TARGET (base_parser 
             ${CMAKE_SOURCE_DIR}/${src}/parser/base_parser.y 
             ${CMAKE_SOURCE_DIR}/${src}/parser/base_parser.c
             HEADER
             ${CMAKE_SOURCE_DIR}/${src}/parser/base_parser.h
             VERBOSE
             ${CMAKE_SOURCE_DIR}/${src}/parser/base_parser.output
             COMPILE_FLAGS
             "-t" # Enable debugging.
             )

set (HEADERS
    ${src}/state_machine/state_machine.h
    ${src}/lexer/lexical_state_machine.h
    ${src}/lexer/lexer.h
    ${src}/lexer/token.h
    ${src}/scope/symrec.h
    ${src}/scope/darray.h
    ${src}/scope/type.h
    ${src}/portability/portability.h
    ${src}/code_gen/generator.h
    ${src}/code_gen/x86.h
    ${src}/code_gen/wml.h
    ${src}/code_gen/stringbuilder.h
    )

set (SOURCES
    ${src}/main.c
    ${src}/lexer/lexical_state_machine.c
    ${src}/state_machine/state_machine.c
    ${src}/lexer/token.c
    ${src}/lexer/lexer.c
    ${src}/scope/symrec.c
    ${src}/scope/type.c
    ${src}/code_gen/x86.c
    ${src}/code_gen/wml.c
    ${src}/code_gen/stringbuilder.c
    ${src}/code_gen/generator.c
    )

if(MSVC)
    source_group (headers\\state_machine FILES
                 ${src}/state_machine/state_machine.h
                 )

    source_group (headers\\lexer FILES 
                 ${src}/lexer/lexical_state_machine.h
                 ${src}/lexer/token.h 
                 ${src}/lexer/lexer.h
                 )

    source_group (headers\\parser FILES 
                 ${src}/parser/parser.h
                 ${CMAKE_SOURCE_DIR}/${src}/parser/base_parser.h
                 )

    source_group (headers\\code_gen FILES 
                 ${src}/code_gen/generator.h
                 ${src}/code_gen/x86.h
                 ${src}/code_gen/wml.h
                 ${src}/code_gen/stringbuilder.h
                 )

    source_group (headers\\portability FILES 
                 ${src}/portability/portability.h
                 )

    source_group (headers\\scope FILES 
                 ${src}/scope/symrec.h
                 ${src}/scope/type.h
                 ${src}/scope/khash.h
                 ${src}/scope/darray.h
                 )
    source_group (sources FILES ${src}/main.c)
    source_group (sources\\state_machine FILES ${src}/state_machine/state_machine.c)
    source_group (sources\\parser FILES ${src}/parser/base_parser.c)
    source_group (sources\\scope FILES 
                 ${src}/scope/symrec.c
                 ${src}/scope/type.c
                 )
    source_group (sources\\lexer FILES 
                 ${src}/lexer/token.c
                 ${src}/lexer/lexer.c
                 ${src}/lexer/lexical_state_machine.c
                 )
    source_group (sources\\code_gen FILES 
                 ${src}/code_gen/generator.c
                 ${src}/code_gen/x86.c
                 ${src}/code_gen/wml.c
                 ${src}/code_gen/stringbuilder.c
                 )
endif()

if (MSVC)
    set_source_files_properties (${SOURCES} ${BISON_base_parser_OUTPUTS} PROPERTIES LANGUAGE CXX)
else ()
endif ()

add_executable (gp-- 
               ${SOURCES} 
               ${HEADERS} 
               ${BISON_base_parser_OUTPUTS}
               )

if (MSVC)
    set_property (TARGET gp-- PROPERTY LINKER_LANGUAGE CXX)
else ()
endif ()
