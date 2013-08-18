cmake_minimum_required (VERSION 2.6)

project (gp-- C CXX)

set (src src)

include_directories(${src})
#                    ${src}/state_machine,
#                    ${src}/lexical)
set (HEADERS
    ${src}/state_machine.h
    ${src}/lexical_state_machine.h
    ${src}/lexical.h
    ${src}/token.h)
#message(${MAKE_CXX_SOURCE_FILE_EXTENSIONS})
SET_SOURCE_FILES_PROPERTIES(main.cc PROPERTIES LANGUAGE CXX)

add_executable (gp-- ${src}/main.cc ${HEADERS})
set_property(TARGET gp-- PROPERTY LINKER_LANGUAGE C) 
