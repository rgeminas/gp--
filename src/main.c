#include "lexer/lexer.h"
#include "parser/parser.h"

int 
main(size_t argc, char** argv)
{
    // Make whitespace/comment skipper and tokenizer.
    STATE_MACHINE* sm = make_full_tokenizer(tokens, 22, keywords, 25);
    STATE_MACHINE* wsssm = make_nontoken_skipper();

    FILE* f;
    if(argc == 1)
    {
        fprintf(stderr, "Usage: gp-- source_file.p");
    }
    else
        f = fopen(argv[1], "r");
    
    while(1)
    {
        TOKEN p = next_token_file(f, sm, wsssm);
        printf("%s: %d\n", (char*) p.token_value, p.type);
        if(p.type==T_EOF) break;
    }
    return 0;    
}
