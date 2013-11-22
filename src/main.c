#include "lexer/lexer.h"
#include "parser/base_parser.h"
#include "portability/portability.h"
#include "lexer/token.h"
#include "scope/symrec.h"
#include "code_gen/stringbuilder.h"
#include "code_gen/wml.h"
// Bison-defined parsing function.
extern int yyparse(void);

extern PAIR tokens[];
extern PAIR keywords[];

// Token recognizer.
extern STATE_MACHINE* _sm; 
// Whitespace skipper.
extern STATE_MACHINE* _wssm;
// File to be parsed.
extern FILE* _f;

extern char** secondary_tokens;
extern size_t sec_i;
extern size_t sec_alloc;

extern int yydebug;

int
main(int argc, char** argv)
{
    yydebug = 0;
    if (argc == 1)
    {
        fprintf(stderr, "Usage: gp-- [--debug] [--help] source_file.pa\n");
        return 1;
    }
    else
    {
        // Override this with something else someday
        init_wml();
        for (int i = 1; i < argc; i++)
        {
            if (!strcmp(argv[i], "--help"))
            {
                puts("Usage: gp-- [--debug] [--help] source_file.pa\n");
                puts("Options:");
                puts("--help: Show help.");
                puts("--debug: Show bison automaton trace.");
                return 0;
            }
            else if (!strcmp(argv[i], "--debug"))
            {
                yydebug = 1;
            }
            else if (!strncmp(argv[i], "-", 1))
            {
                printf("Unknown option %s.\n", argv[i]);
                return 1;
            }
            else
            {
                FILE* f = fopen(argv[i], "r");
                if (f == NULL)
                {
                    printf("Could not open file: %s.\n", argv[i]);
                    return 1;
                }
                _f = f;
            }
        }
    }

    _sm = make_full_tokenizer(tokens, 22, keywords, 26);
    _wssm = make_nontoken_skipper(); 

    sec_alloc = 3;
    secondary_tokens = (char**) malloc(3 * sizeof(char*));
    secondary_tokens[0] = "print";
    secondary_tokens[1] = "arg";
    sec_i++;
    sec_i++;

    if (!yyparse())
    {
        puts("Parsing successful! Wheeeeee!");
    }
    else
    {
        puts("Parsing unsuccessful.");
    }
    fclose(_f);
    return 0;
}
