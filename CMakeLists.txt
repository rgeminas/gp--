cmake_minimum_required (VERSION 2.8)

if(MSVC)
project (gp-- C CXX)
else()
project (gp-- C)
set (CMAKE_C_FLAGS "-std=c99")
endif()

set (src src)

#set (CMAKE_BUILD_TYPE Debug)

include_directories (${src})

set (HEADERS
    ${src}/state_machine/state_machine.h
    ${src}/lexer/lexical_state_machine.h
    ${src}/lexer/lexer.h
    ${src}/lexer/token.h
    ${src}/parser/parser.h
    ${src}/portability/portability.h)

set (SOURCES
    ${src}/main.c)

source_group (headers\\state_machine FILES
             ${src}/state_machine/state_machine.h)

source_group (headers\\lexer FILES 
             ${src}/lexer/lexical_state_machine.h
             ${src}/lexer/token.h 
             ${src}/lexer/lexer.h)

source_group (headers\\parser FILES 
             ${src}/parser/parser.h)

source_group (headers\\portability FILES 
             ${src}/portability/portability.h)

source_group (sources FILES ${src}/main.c)

if (MSVC)
SET_SOURCE_FILES_PROPERTIES (${SOURCES} PROPERTIES LANGUAGE CXX)
endif ()

add_executable (gp-- ${SOURCES} ${HEADERS})

if (MSVC)
set_property (TARGET gp-- PROPERTY LINKER_LANGUAGE CXX)
else ()
set_property (TARGET gp-- PROPERTY LINKER_LANGUAGE C)
endif ()
