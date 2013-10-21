gp--
====

Introduction
------------
gp-- (Gemignani's Pascal-- Compiler) is a compiler for a restricted subset of Pascal (named Pascal--) outputting x86 assembly code, written as an exercise for the Compilers class for the Computer Engineering course taught by professor Alex Garcia in the Military Institute of Engineering (IME) in Spring/2013.

Installation and Usage
----------------------
To compile gp--, you must have cmake and bison installed and a C99-compatible compiler (MSVC, notably, doesn't provide C99 support, but we intend to support it regardless by compiling gp-- with C++ under MSVC).

Usage: $./gp-- source_file.pa [--debug]

Current state
-------------
Currently, lexing, parsing, scope and type checks are complete, though code generation doesn't happen yet - Soon(TM).

Lexer
-----
The lexer uses a custom simple state machine implementation, and constructs a tokenizer based on very simple rules: it is given a set of keywords and a set of non-keyword tokens. Alphanumeric tokens that aren't keywords are signaled as identifiers. 

Parser
------
The parser is an LALR(1) parser written using bison. We use [khash][1], by attractive chaos to construct the symbol tables.

[1]: https://github.com/attractivechaos/klib
