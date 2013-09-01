#include "lexer/lexer.h"
#include "parser/parser.h"
#include "portability/portability.h"

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

extern int yydebug;

int
main(int argc, char** argv)
{
    yydebug = 0;
    if(argc == 1)
    {
        fprintf(stderr, "Usage: gp-- [-d] [--help] source_file.p\n");
        return 1;
    }
    else
    {
        for(int i=1; i < argc; i++)
        {
            if(!strcmp(argv[i], "--help"))
            {
                puts("Usage: gp-- [-d] [--help] source_file.p\n");
                puts("Options:");
                puts("-d/--debug: Show bison automaton trace.");
                puts("--help: Show help.");
                return 0;
            }
            else if(!strcmp(argv[i], "-d") || !strcmp(argv[i], "--debug"))
            {
                yydebug = 1;
            }
            else
            {
                FILE* f = fopen(argv[i], "r");
                _f = f;
            }
        }
    }
    
    _sm = make_full_tokenizer(tokens, 22, keywords, 27);
    _wssm = make_nontoken_skipper(); 

    if(!yyparse())
    {
        puts("!Awwww yiss... who da parser? who da parser?");
    }
    else
    {
        puts("... nooOO!");
    }
    return 0;
}
