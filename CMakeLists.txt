cmake_minimum_required (VERSION 2.8)

project (gp-- CXX)
set (src src)

include_directories(${src})
#                    ${src}/state_machine,
#                    ${src}/lexical)

set (HEADERS
    ${src}/state_machine.h
    ${src}/lexical_state_machine.h
    ${src}/lexer.h
    ${src}/token.h
    ${src}/parser.h)

source_group (headers\\state_machine FILES ${src}/state_machine.h ${src}/lexical_state_machine.h)
source_group (headers\\lexer FILES ${src}/token.h ${src}/lexer.h)
source_group (headers\\parser FILES ${src}/parser.h)
source_group (sources FILES ${src}/main.cc)

add_executable (gp-- ${src}/main.cc ${HEADERS})
set_property(TARGET gp-- PROPERTY LINKER_LANGUAGE CXX) 
