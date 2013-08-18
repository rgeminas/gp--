cmake_minimum_required (VERSION 2.8)

project (gp-- CXX)
set (src src)

include_directories(${src})
#                    ${src}/state_machine,
#                    ${src}/lexical)

set (HEADERS
    ${src}/state_machine/state_machine.h
    ${src}/lexer/lexical_state_machine.h
    ${src}/lexer/lexer.h
    ${src}/lexer/token.h
    ${src}/parser/parser.h)

source_group (headers\\state_machine FILES
              ${src}/state_machine/state_machine.h)
source_group (headers\\lexer FILES 
             ${src}/lexer/lexical_state_machine.h
             ${src}/lexer/token.h 
             ${src}/lexer/lexer.h)
source_group (headers\\parser FILES 
             ${src}/parser/parser.h)
source_group (sources FILES ${src}/main.cc)

add_executable (gp-- ${src}/main.cc ${HEADERS})
set_property(TARGET gp-- PROPERTY LINKER_LANGUAGE CXX) 
