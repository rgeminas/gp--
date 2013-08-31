#ifndef __LEXICAL_STATE_MACHINE_H
#define __LEXICAL_STATE_MACHINE_H

typedef struct TOKEN
{
    int type;
    char* token_value;
} TOKEN;

STATE_MACHINE*
make_full_tokenizer(PAIR* unsorted_tokens,
                    size_t num_tokens,
                    PAIR* keywords,
                    size_t num_keywords);

STATE_MACHINE*
make_nontoken_skipper(void);


#endif // __LEXICAL_STATE_MACHINE_H