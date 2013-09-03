gp--
====

Introduction
------------
gp-- (Gemignani's Pascal-- Compiler) is a compiler for a restricted subset of Pascal (named Pascal--), written as an exercise for the Compilers class for the Computer Engineering course taught by professor Alex Garcia in the Military Institute of Engineering (IME) in Fall/2013.

Installation and usage
----------------------
To compile gp--, you must have cmake and bison installed and a C99-compatible compiler (MSVC, notably, doesn't provide C99 support, but we intend to support it regardless by compiling gp-- with C++ under MSVC. Currently we do NOT support MSVC).

Usage: $./gp-- source_file.pa

Current State
-------------
Currently, lexing and parsing are complete, though code generation doesn't happen yet and there is no AST construction as of now.

Lexer
-----
The lexer uses a custom simple state machine implementation, and constructs a tokenizer based on very simple rules: it is given a set of keywords and a set of non-keyword tokens. Alphanumeric tokens that aren't keywords are signaled as identifiers. 

Parser
------
The parser is an LALR(1) parser written using bison.
