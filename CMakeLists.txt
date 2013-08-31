cmake_minimum_required (VERSION 2.8)

find_package(BISON REQUIRED)

if(MSVC)
project (gp-- C CXX)
else()
project (gp-- C)
set (CMAKE_C_FLAGS "-std=c99")
endif()

set (src src)

#set (CMAKE_BUILD_TYPE Debug)

include_directories (${src})

message (STATUS ${CMAKE_SOURCE_DIR})

BISON_TARGET (base_parser 
             ${CMAKE_SOURCE_DIR}/${src}/parser/base_parser.y 
             ${CMAKE_SOURCE_DIR}/${src}/parser/base_parser.tab.c

             VERBOSE
             ${CMAKE_SOURCE_DIR}/${src}/parser/base_parser.output
             COMPILE_FLAGS
             "--defines=${src}/parser/base_parser.h"
             )



set (HEADERS
    ${src}/state_machine/state_machine.h
    ${src}/lexer/lexical_state_machine.h
    ${src}/lexer/lexer.h
    ${src}/lexer/token.h
    ${src}/parser/parser.h
    ${src}/parser/base_parser.h
    ${src}/portability/portability.h
    )

set (SOURCES
    ${src}/main.c
    ${src}/lexer/lexical_state_machine.c
    ${src}/state_machine/state_machine.c
    ${src}/lexer/token.c
    ${src}/lexer/lexer.c
    ${src}/parser/base_parser.tab.c
    )

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
             ${src}/parser/base_parser.h
             )

source_group (headers\\portability FILES 
             ${src}/portability/portability.h
             )

source_group (sources FILES ${src}/main.c)
source_group (sources\\state_machine FILES ${src}/state_machine/state_machine.c)
source_group (sources\\parser FILES ${src}/parser/base_parser.tab.c)
source_group (sources\\lexer FILES 
             ${src}/lexer/token.c
             ${src}/lexer/lexer.c
             ${src}/lexer/lexical_state_machine.c
             )
             

if (MSVC)
set_source_files_properties (${SOURCES} PROPERTIES LANGUAGE CXX)
endif ()

add_executable (gp-- ${SOURCES} ${HEADERS} 
               #${BISON_base_parser_OUTPUTS}
               )

if (MSVC)
set_property (TARGET gp-- PROPERTY LINKER_LANGUAGE CXX)
else ()
set_property (TARGET gp-- PROPERTY LINKER_LANGUAGE C)
endif ()
