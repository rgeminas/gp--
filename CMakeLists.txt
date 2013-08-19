cmake_minimum_required(VERSION 2.8)

project(gp--)
set(CMAKE_C_FLAGS "-std=c99")

set(src src)
#set (CMAKE_BUILD_TYPE Debug)
include_directories(${src})
#                    ${src}/state_machine,
#                    ${src}/lexical)

set(HEADERS
    ${src}/state_machine/state_machine.h
    ${src}/lexer/lexical_state_machine.h
    ${src}/lexer/lexer.h
    ${src}/lexer/token.h
    ${src}/parser/parser.h)

source_group(headers\\state_machine FILES
              ${src}/state_machine/state_machine.h)
source_group(headers\\lexer FILES
             ${src}/lexer/lexical_state_machine.h
             ${src}/lexer/token.h
             ${src}/lexer/lexer.h)

source_group(headers\\parser FILES
             ${src}/parser/parser.h)
source_group(sources FILES ${src}/main.c)

if(MSVC)
set_source_files_properties(main.c PROPERTIES LANGUAGE CXX)
endif()

add_executable (gp-- ${src}/main.c ${HEADERS})

if(MSVC)
#SET_SOURCE_FILES_PROPERTIES( ${HEADERS} PROPERTIES LANGUAGE CXX )
set_property(TARGET gp-- PROPERTY LINKER_LANGUAGE CXX)
else()
set_property(TARGET gp-- PROPERTY LINKER_LANGUAGE C)
endif()
