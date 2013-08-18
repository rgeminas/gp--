#include "lexer/lexer.h"
#include "parser/parser.h"

int 
main(size_t argc, char** argv)
{
    // Make whitespace/comment skipper and tokenizer.
    STATE_MACHINE* sm = make_full_tokenizer(tokens, 22, keywords, 23);
    STATE_MACHINE* wsssm = make_nontoken_skipper();

    FILE* f;
    if(argc == 1)
    {
        f = fopen("main.p", "r");
        if(f == NULL) return 0;
    }
    else
        f = fopen(argv[1], "r");
    
    while(true)
    {
        TOKEN p = next_token_file(f, sm, wsssm);
        printf("%s: %d\n", (char*) p.token_value, p.type);
        if(p.type==T_EOF) break;
    }
    return 0;
    
    /*
    //ISSO FUNCIONA
    if(argc == 1)
    {
        return 1;
    }
    char* a = argv[1];
    //char* a = "integer main(void) {brasil} begin var x = 3 >= 4. .. . end";
    void* memarea = malloc(sizeof(void*));
    void** return_ptr = &memarea;
    size_t* size = NULL;
    int return_type = 2;

    do
    {
        return_type = next_token(&a, return_ptr, size);
        if (return_type == T_INT_CONST)
        {
            printf("Retorno: %d, valor: %d\n", return_type, *((int*)*return_ptr));
            //free(*return_ptr);
        }
        else if (return_type == T_REAL_CONST)
        {
            printf("Retorno: %d, valor: %f\n", return_type, *((float*)*return_ptr));
            //
        }
        else if (return_type == T_ID)
        {
            printf("Retorno: %d, valor: %s\n", return_type, (char*)*return_ptr);
            free(*return_ptr);
        }
        else
        {
            printf("Retorno: %d\n", return_type);
        }
        //free(*return_ptr);
    } while(return_type != T_EOF && return_type != T_INVALID);
    //puts(a);
	return 0;*/
}
