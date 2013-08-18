/*
Definition of tokens returned by the lexical analysis compiler pass.
*/

#ifndef __TOKEN_H
#define __TOKEN_H

#define TOKEN_TYPE int

#define T_EOF                  0
#define T_ID                   1
#define T_INT_CONST            2
#define T_REAL_CONST           3
#define T_BOOLEAN_CONST        4
#define T_PROGRAM              5
#define T_PROCEDURE            6
#define T_BEGIN                7
#define T_END                  8
#define T_IF                   9
#define T_THEN                 10
#define T_ELSE                 11
#define T_WHILE                12
#define T_DO                   13
#define T_ASSIGN               14
#define T_PLUS                 15
#define T_MINUS                16
#define T_TIMES                17
#define T_DIVIDE               18
#define T_DIV                  19
#define T_MOD                  20
#define T_PLUS_EQ              21
#define T_MINUS_EQ             22
#define T_TIMES_EQ             23
#define T_AND                  24
#define T_OR                   25
#define T_NOT                  26
#define T_DIVIDE_EQ            27
#define T_VAR                  28
#define T_INTEGER              29
#define T_REAL                 30
#define T_BOOLEAN              31
#define T_COLON                32
#define T_SEMICOLON            33
#define T_COMMA                34
#define T_PERIOD               35
#define T_DOUBLE_PERIOD        36
#define T_LBRACKET             37
#define T_RBRACKET             38
#define T_CONST                39
#define T_REPEAT               40
#define T_UNTIL                41
#define T_EQ                   42
#define T_DIF                  43
#define T_LT                   44
#define T_LEQ                  45
#define T_GT                   46
#define T_GEQ                  47
#define T_REWIND_ONE          254
#define T_INVALID             255

typedef struct PAIR
{
    char* token;
    TOKEN_TYPE type;
} PAIR;

PAIR tokens[] = {
    {":=", T_ASSIGN}, 
    {"+", T_PLUS}, 
    {"+=", T_PLUS_EQ}, 
    {"-", T_MINUS}, 
    {"-=", T_MINUS_EQ},  
    {"*", T_TIMES}, 
    {"*=", T_TIMES_EQ}, 
    {"/", T_DIVIDE}, 
    {"/=", T_DIVIDE_EQ}, 
    {"<=", T_LEQ}, 
    {">=", T_GEQ}, 
    {">", T_GT}, 
    {"<", T_LT}, 
    {"<>", T_DIF}, 
    {"=", T_EQ},
    {";", T_SEMICOLON},
    {":", T_COLON},
    {".", T_PERIOD},
    {"..", T_DOUBLE_PERIOD},
    {"(", T_LBRACKET},
    {")", T_RBRACKET},
    {",", T_COMMA}
};

PAIR keywords[] = {
    {"program", T_PROGRAM },
    {"procedure", T_PROCEDURE },
    {"begin", T_BEGIN },
    {"end", T_END },
    {"if", T_IF },
    {"else", T_ELSE },
    {"while", T_WHILE },
    {"do", T_DO },
    {"div", T_DIV },
    {"mod", T_MOD },
    {"and", T_AND },
    {"or", T_OR },
    {"not", T_NOT },
    {"var", T_VAR },
    {"integer", T_INTEGER },
    {"boolean", T_BOOLEAN },
    {"real", T_REAL },
    {"const", T_CONST },
    {"repeat", T_REPEAT },
    {"until", T_UNTIL },
    {"if", T_IF},
    {"then", T_THEN},
    {"else", T_ELSE}
};

#endif // __TOKEN_H
