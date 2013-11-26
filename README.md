gp--
====

Introduction
------------
gp-- (Gemignani's Pascal-- Compiler) is a compiler for a restricted subset of Pascal (named Pascal--) outputting assembly code for the WMLScript virtual machine, written as an exercise for the Compilers class for the Computer Engineering course taught by professor Alex Garcia in the Military Institute of Engineering (IME) in Spring/2013.

Installation and Usage
----------------------
To compile gp--, you must have cmake and bison installed and a C99-compatible compiler (MSVC, notably, doesn't provide C99 support, but we intend to support it regardless by compiling gp-- with C++ under MSVC).

Usage: $./gp-- source_file.pa [--debug] > output.asm

The assembler is not provided due to licensing issues (namely, I don't know whether I can bundle it here or not).

Current State of the Project
-------------
Project is somewhat feature-complete, generating unoptimized code. Optimization isn't going to happen.

Lexer
-----
The lexer uses a custom simple state machine implementation, and constructs a tokenizer based on very simple rules: it is given a set of keywords and a set of non-keyword tokens. Alphanumeric tokens that aren't keywords are signaled as identifiers. 

Parser and Static Checking
--------------------------
The parser is an LALR(1) parser written using bison. We use [khash][1], by attractive chaos to construct the symbol tables.

Code Generation
---------------
The code is built implicitly in the derivation tree, and output once at the end.

[1]: https://github.com/attractivechaos/klib
