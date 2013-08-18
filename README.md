gp--
====

Introduction
------------
gp-- (Gemignani's Pascal-- Compiler) is a compiler for a restricted subset of Pascal (named Pascal--), written as an exercise for the Compilers class for the Computer Engineering course taught by professor Alex Garcia in the Military Institute of Engineering (IME) in Fall/2013.

Installation and usage
----------------------
To compile gp--, you must have cmake installed and a C99-compatible compiler (MSVC, notably, doesn't provide C99 support).

Usage: $./gp-- source_file.p

Current State
-------------
Currently, only syntactic analysis is being done. An LR(1) parser for the language will be written Soon(TM).

Lexer
-----
The lexer uses a custom simple state machine implementation, and constructs a tokenizer based on very simple rules: it is given a set of keywords and a set of non-keyword tokens. Alphanumeric tokens that aren't keywords are signaled as identifiers. 
