#include "lexer/lexer.h"
#include "parser/parser.h"
#include "portability/portability.h"

int
main(int argc, char** argv)
{
    // Make whitespace/comment skipper and tokenizer.
    STATE_MACHINE* sm = make_full_tokenizer(tokens, 22, keywords, 27);
    STATE_MACHINE* wsssm = make_nontoken_skipper();

    char** secondary_tokens_char = (char**) malloc(sizeof(char*));
    size_t sec_i = 0;
    size_t sec_alloc = 1;
    
    int* secondary_tokens_int = (int*) malloc(sizeof(int));  
    float* secondary_tokens_float = (float*) malloc(sizeof(float));

    FILE* f;
    if(argc == 1)
    {
        //f = fopen("massadetesteslexica.txt", "r");
        fprintf(stderr, "Usage: gp-- source_file.p\n");
        return 1;
    }
    else
        f = fopen(argv[1], "r");

    while(1)
    {
        TOKEN p = next_token_file(f, sm, wsssm);
        //printf("%s: %d\n", (char*) p.token_value, p.type);
        if(p.type == T_EOF) break;
        if(p.type == T_ID)
        {
            size_t i;
            for(i=0; i < sec_i; i++)
            {
                if(!stricmp(secondary_tokens_char[i], (char*) p.token_value)) break;
            }
            if(i >= sec_i)
            {
                if(sec_i > sec_alloc)
                {
                    sec_alloc << 1;
                    secondary_tokens_char = (char**) realloc(secondary_tokens_char, sec_alloc * sizeof(char*));
                }
                secondary_tokens_char[sec_i++] = (char*) p.token_value;
                printf("%s: %d, %d\n", (char*) p.token_value, p.type, sec_i - 1);
            }
            else
            {
                printf("%s: %d, %d\n", (char*) p.token_value, p.type, i);
            }
        }
        else if(p.type == T_INT_CONST)
        {
            int value = atoi((char*) p.token_value);
            printf("%s: %d, %d\n", (char*) p.token_value, p.type, value);
        }
        else if(p.type == T_REAL_CONST)
        {
            float value = atof((char*) p.token_value);
            printf("%s: %d, %f\n", (char*) p.token_value, p.type, value);
        }
        else if(p.type == T_BOOLEAN_CONST)
        {
            if(stricmp((char*)p.token_value, "true"))
            {
                printf("%s: %d, %d\n", (char*) p.token_value, p.type, 1);
            }
            else
            {
                printf("%s: %d, %d\n", (char*) p.token_value, p.type, 0);
            }
        }
        else // 2ndary token unneeded
        {
            free(p.token_value);
            printf("%s: %d\n", (char*) p.token_value, p.type);
        }
    }
    return 0;
}
