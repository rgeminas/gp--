#include "state_machine/state_machine.h"
#include "lexer/token.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char* const alpha = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_";
const char* const numerics = "1234567890";
const char* const alphanumerics = "1234567890abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_";
const char* const whitespace = "\t\v\r\n ";

int
compare_int(int a,
            int b)
{
    if(a <  b) return -1;
    if(a == b) return 0;
    if(a >  b) return 1;
    return 0;
}

int
compare_string_size(const void* p1,
                    const void* p2)
{
    int x1 = strlen(((PAIR*) p1)->token), x2 = strlen(((PAIR*) p2)->token);
    return compare_int(x1, x2);
}
/*
Makes a state that recognizes the the string value from
the character in 'depth' onwards, while having branches to
any other tokens that have are the same up to 'depth'
characters. I.e., if depth=0 and value = 'void', and
there are also other tokens starting with 'v', the
returned state will recognize 'v', and have branches to
'void' and all other tokens starting with 'v', creating
a tree structure that can be traversed to find the TOKEN_TYPE
of a string.
*/
STATE*
make_basic_tokenizer_state(const PAIR * tokens,
                           size_t num_tokens,
                           char* value,
                           TOKEN_TYPE type,
                           size_t depth)
{
    STATE initial_state_val = { value[depth], NULL, 0, 0, type  };
    for(size_t i=0; i<num_tokens; i++)
    {
        char* token = tokens[i].token;
        TOKEN_TYPE next_type = tokens[i].type;

        if(!strncmp(token, value, depth + 1))
        {
            if(token[depth + 1] == '\0') continue;
            else
            {
                size_t j;
                for(j=0; j<i; j++)
                {
                    char* token_to_check = tokens[j].token;
                    // Comparing until position depth + 1 is the same as comparing depth + 2 positions
                    if(!strncmp(token, token_to_check, depth + 2)) break;
                }
                if(j < i) continue;
            }
            if(strlen(token) > depth + 2) next_type = T_INVALID;
            STATE* to_add = make_basic_tokenizer_state(tokens, num_tokens, token, next_type, depth + 1);
            add_transition(&initial_state_val, to_add);
        }
    }
    STATE* state = (STATE*) malloc(sizeof(STATE));
    memcpy(state, &initial_state_val, sizeof(STATE));
    return state;
}

/*
Makes a state machine recognize any token in unsorted_tokens,
returning T_ID for incomplete tokens.
*/
void
add_basic_tokenizing(STATE_MACHINE* sm,
                     PAIR* unsorted_tokens,
                     size_t num_tokens)
{
    PAIR* tokens = (PAIR*) malloc(num_tokens * sizeof(PAIR));
    memcpy(tokens, unsorted_tokens, num_tokens * sizeof(PAIR));
    qsort(tokens, num_tokens, sizeof(PAIR), compare_string_size);

    for(size_t i=0; i<num_tokens; i++)
    {
        size_t j;
        for(j=0; j<i; j++)
        {
            if(tokens[i].token[0] == tokens[j].token[0]) break;
        }
        if(j < i) continue;
        STATE* s = make_basic_tokenizer_state(tokens, num_tokens, tokens[i].token, tokens[i].type, 0);
        add_transition(sm->initial_state, s);
    }
    free(tokens);
//    return sm;
}

/*
Makes a state machine recognize any token in unsorted_tokens,
returning T_ID for incomplete tokens.
*/
STATE* 
make_basic_keyword_tokenizer_state(const PAIR * tokens,
                           size_t num_tokens,
                           char* value,
                           TOKEN_TYPE type,
                           size_t depth)
{
    STATE initial_state_val = { value[depth], NULL, 0, 0, type  };
    for(size_t i=0; i<num_tokens; i++)
    {
        char* token = tokens[i].token;
        TOKEN_TYPE next_type = tokens[i].type;

        if(!strncmp(token, value, depth + 1))
        {
            if(token[depth + 1] == '\0') continue;
            else
            {
                size_t j;
                for(j=0; j<i; j++)
                {
                    char* token_to_check = tokens[j].token;
                    // Comparing until position depth + 1 is the same as comparing depth + 2 positions
                    if(!strncmp(token, token_to_check, depth + 2)) break;
                }
                if(j < i) continue;
            }
            if(strlen(token) > depth + 2) next_type = T_ID;
            STATE* to_add = make_basic_keyword_tokenizer_state(tokens, num_tokens, token, next_type, depth + 1);
            add_transition(&initial_state_val, to_add);
        }
    }
    STATE* state = (STATE*) malloc(sizeof(STATE));
    memcpy(state, &initial_state_val, sizeof(STATE));
    return state;
}

/*
Creates a state machine that recognizes any token in unsorted_tokens,
and returns T_ID for an incomplete token.
*/
void
add_basic_keyword_tokenizing(STATE_MACHINE* sm,
                     PAIR* unsorted_tokens,
                     size_t num_tokens)
{
    PAIR* tokens = (PAIR*) malloc(num_tokens * sizeof(PAIR));
    memcpy(tokens, unsorted_tokens, num_tokens * sizeof(PAIR));
    qsort(tokens, num_tokens, sizeof(PAIR), compare_string_size);

    for(size_t i=0; i<num_tokens; i++)
    {
        size_t j;
        for(j=0; j<i; j++)
        {
            if(tokens[i].token[0] == tokens[j].token[0]) break;
        }
        if(j < i) continue;
        TOKEN_TYPE next_type = tokens[i].type;
        if(strlen(tokens[i].token) > 1) next_type = T_ID;
        STATE* s = make_basic_keyword_tokenizer_state(tokens, num_tokens, tokens[i].token, next_type , 0);
        add_transition(sm->initial_state, s);
    }
    free(tokens);
}


/*
Makes the state machine capable of recognizing real and integer numbers.
*/
void
add_number_recognition(STATE_MACHINE* sm)
{
    STATE* numeric_states = (STATE*) malloc(10 * sizeof(STATE));
    STATE* real_states = (STATE*) malloc(10 * sizeof(STATE));
    // State for reading a dot after a number
    STATE s_real_val = { '.', NULL, 0, 0, T_REAL_CONST };
    STATE* s_real = (STATE*) malloc(sizeof(STATE));
    memcpy(s_real, &s_real_val, sizeof(STATE));

    //Constructs states for real numbers after the dot.
    for(size_t i=0; i<10; i++)
    {
        STATE s = { numerics[i], NULL, 0, 0, T_REAL_CONST };
        memcpy(real_states + i, &s, sizeof(STATE));
    }
    // Adds recursive transitions to each other real number state
    for(size_t i=0; i<10; i++)
    {
        for(size_t j=0; j<10; j++)
        {
            // Allows a digit to be followed by a digit.
            add_transition(real_states + i, real_states + j);
        }
    }
    // Allows a dot to be followed by any digit.
    for(size_t i=0; i<10; i++)
    {
        add_transition(s_real, real_states + i);
    }
    // Constructs states for integers.
    for(size_t i=0; i<10; i++)
    {
        STATE s = { numerics[i], NULL, 0, 0, T_INT_CONST };
        memcpy(numeric_states + i, &s, sizeof(STATE));
    }
    for(size_t i=0; i<10; i++)
    {
        for(size_t j=0; j<10; j++)
        {
            // Allows a digit to be followed by a digit.
            add_transition(numeric_states + i, numeric_states + j);
        }
        // An integer followed by a dot becomes a real.
        add_transition(numeric_states + i, s_real);
    }
    for(size_t i=0; i<10; i++)
    {
        add_transition((sm)->initial_state, numeric_states + i);
    }
    add_transition((sm)->initial_state, s_real);
}

/*
Adds the non-conflicting id_alphanumeric_states to s for recognizing
identifiers, such as function names et al.
*/
void
add_identifier_states(STATE* s,
                      STATE* id_alphanumeric_states,
                      size_t num_alphanum)
{
    for(size_t i=0; i<num_alphanum; i++)
    {
        size_t j;
        for(j=0; j<s->num_transitions; j++)
        {
            if(s->allowed_transitions[j]->state == alphanumerics[i]) break;
        }
        if(j < s->num_transitions) continue;
        add_transition(s, id_alphanumeric_states + i);
        for(j=0; j<s->num_transitions; j++)
        {
            add_identifier_states(s->allowed_transitions[i], id_alphanumeric_states, num_alphanum);
        }
    }
}

void
add_keyword_recognition(STATE_MACHINE* sm,
                        PAIR* unsorted_keywords,
                        size_t num_keywords)
{
    size_t num_alpha = strlen(alpha), num_alphanum = strlen(alphanumerics);

    PAIR* keywords = (PAIR*) malloc(num_keywords * sizeof(PAIR));
    memcpy(keywords, unsorted_keywords, num_keywords * sizeof(PAIR));
    qsort(keywords, num_keywords, sizeof(PAIR), compare_string_size);

    add_basic_keyword_tokenizing(sm, unsorted_keywords, num_keywords);

    STATE* id_alpha_states = (STATE*) malloc(num_alpha * sizeof(STATE));
    STATE* id_alphanumeric_states = (STATE*) malloc(num_alphanum * sizeof(STATE));

    for(size_t i=0; i<num_alpha; i++)
    {
        STATE id_alpha_state_val = { alpha[i], NULL, 0, 0, T_ID };
        memcpy(id_alpha_states + i, &id_alpha_state_val, sizeof(STATE));
    }
    for(size_t i=0; i<num_alphanum; i++)
    {
        STATE id_alphanum_state_val = { alphanumerics[i], NULL, 0, 0, T_ID };
        memcpy(id_alphanumeric_states + i, &id_alphanum_state_val, sizeof(STATE));
    }
    for(size_t i=0; i<num_alpha; i++)
    {
        for(size_t j=0; j<num_alphanum; j++)
        {
            // An alpha can be followed by any alphanum
            add_transition(id_alpha_states + i, id_alphanumeric_states + j);
        }
    }
    for(size_t i=0; i<num_alphanum; i++)
    {
        for(size_t j=0; j<num_alphanum; j++)
        {
            // An alphanum can be followed by any alphanum;
            add_transition(id_alphanumeric_states + i, id_alphanumeric_states + j);
        }
    }
    for(size_t i=0; i<num_alpha; i++)
    {
        size_t j;
        for(j=0; j<sm->initial_state->num_transitions; j++)
        {
            if(sm->initial_state->allowed_transitions[j]->state == alpha[i]) break;
        }
        if(j<sm->initial_state->num_transitions) continue;
        add_transition(sm->initial_state, id_alpha_states + i);
        for(j=0; j<sm->initial_state->num_transitions; j++)
        {
            // If the character in the state is an alpha, it should be able to segue
            // into the identifier states.
            if(strchr(alpha, sm->initial_state->allowed_transitions[i]->state))
            {
                // Adds identifier recognition to the first states, recursion will make
                // this bubble up to their transitioned states.
                add_identifier_states(sm->initial_state->allowed_transitions[i], id_alphanumeric_states, num_alphanum);
            }
        }
    }
}

STATE_MACHINE*
make_full_tokenizer(PAIR* unsorted_tokens,
                    size_t num_tokens,
                    PAIR* keywords,
                    size_t num_keywords)
{
    STATE_MACHINE* sm = (STATE_MACHINE*) malloc(sizeof(STATE_MACHINE));
    STATE* initial_state = (STATE*) malloc(sizeof(STATE));

    STATE initial_state_val = { 0, NULL, 0, 0, T_INVALID  };
    memcpy(initial_state, &initial_state_val, sizeof(STATE));

    STATE_MACHINE sm_val = { initial_state, initial_state };
    memcpy(sm, &sm_val, sizeof(STATE_MACHINE));

    add_basic_tokenizing(sm, unsorted_tokens, num_tokens);
    add_number_recognition(sm);
    add_keyword_recognition(sm, keywords, num_keywords);
    return sm;
}

/*
Returns a state machine that skips brace_comments and whitespace.

TODO: This is fully hand-coded. For the love of GodJesusAllah I've got to
automate this for different comment tags.
*/
STATE_MACHINE*
make_nontoken_skipper(void)
{
    size_t num_whitespace = strlen(whitespace);
    STATE_MACHINE* sm = (STATE_MACHINE*) malloc(sizeof(STATE_MACHINE));

    // Fugly, but necessary because this is fucking c
    STATE* initial_state = (STATE*) malloc(sizeof(STATE));
    STATE* start_comment_brace = (STATE*) malloc(sizeof(STATE));
    STATE* end_comment_brace = (STATE*) malloc(sizeof(STATE));
    STATE* maybe_start_comment_bracket = (STATE*) malloc(sizeof(STATE));
    STATE* start_comment_bracket = (STATE*) malloc(sizeof(STATE));
    STATE* maybe_end_comment_bracket = (STATE*) malloc(sizeof(STATE));
    STATE* end_comment_bracket = (STATE*) malloc(sizeof(STATE));

    STATE initial_state_val = { 0, NULL, 0, 0, T_ID };
    STATE start_comment_brace_val = { '{', NULL, 0, 0, T_INVALID };
    STATE end_comment_brace_val = { '}', NULL, 0, 0, T_ID };
    STATE maybe_start_comment_bracket_val = {'(', NULL, 0, 0, T_REWIND_ONE};
    STATE start_comment_bracket_val = {'*', NULL, 0, 0, T_INVALID};
    STATE maybe_end_comment_bracket_val = {'*', NULL, 0, 0, T_INVALID};
    STATE end_comment_bracket_val = {')', NULL, 0, 0, T_ID};

    memcpy(initial_state, &initial_state_val, sizeof(STATE));
    memcpy(start_comment_brace, &start_comment_brace_val, sizeof(STATE));
    memcpy(end_comment_brace, &end_comment_brace_val, sizeof(STATE));
    memcpy(maybe_end_comment_bracket, &maybe_end_comment_bracket_val, sizeof(STATE));
    memcpy(end_comment_bracket, &end_comment_bracket_val, sizeof(STATE));
    memcpy(start_comment_bracket, &start_comment_bracket_val, sizeof(STATE));
    memcpy(maybe_start_comment_bracket, &maybe_start_comment_bracket_val, sizeof(STATE));

    STATE_MACHINE sm_val = { initial_state, initial_state };
    memcpy(sm, &sm_val, sizeof(STATE_MACHINE));

    add_transition(start_comment_brace, end_comment_brace);
    add_transition(maybe_start_comment_bracket, start_comment_bracket);
    add_transition(start_comment_bracket, maybe_end_comment_bracket);
    add_transition(maybe_end_comment_bracket, end_comment_bracket);

    STATE* states_inside_brace_comments = (STATE*) malloc((0x7e - 0x20 + 4) * sizeof(STATE));

    int c = 0;
    for(char i=0x20; i<=0x7e; i++)
    {
        STATE state = { i, NULL, 0, 0, T_INVALID };
        memcpy(states_inside_brace_comments + c, &state, sizeof(STATE));
        if(i != '}')
        {
            add_transition(start_comment_brace, states_inside_brace_comments + c);
            add_transition(states_inside_brace_comments + c, end_comment_brace);
            c++;
        }
    }
    // Allow tabs, newlines, vtabs and carriage returns inside brace_comments.
    for(size_t i=0; i<4; i++)
    {
        STATE state = { whitespace[i], NULL, 0, 0, T_INVALID };
        memcpy(states_inside_brace_comments + c, &state, sizeof(STATE));
        add_transition(states_inside_brace_comments + c, end_comment_brace);
        c++;
    }
    for(char i=0; i<0x7e - 0x20 + 4; i++)
    {
        for(char j=0; j<0x7e - 0x20 + 4; j++)
        {
            add_transition(states_inside_brace_comments + i, states_inside_brace_comments + j);
        }
    }

    STATE* states_inside_bracket_comments = (STATE*) malloc((0x7e - 0x20 + 4) * sizeof(STATE));
    c = 0;
    for(char i=0x20; i<=0x7e; i++)
    {
        STATE state = { i, NULL, 0, 0, T_INVALID };
        memcpy(states_inside_bracket_comments + c, &state, sizeof(STATE));
        if(i != '*')
        {
            add_transition(start_comment_bracket, states_inside_bracket_comments + c);
            add_transition(states_inside_bracket_comments + c, maybe_end_comment_bracket);
            // If after *, a ) doesn't segue, we are still inside a comment.
            if(i != ')')
                add_transition(maybe_end_comment_bracket, states_inside_bracket_comments + c);
            c++;
        }
    }
    // Allow tabs, newlines, vtabs and carriage returns inside bracket_comments.
    for(size_t i=0; i<4; i++)
    {
        STATE state = { whitespace[i], NULL, 0, 0, T_INVALID };
        memcpy(states_inside_bracket_comments + c, &state, sizeof(STATE));
        add_transition(states_inside_bracket_comments + c, maybe_end_comment_bracket);
        c++;
    }
    for(char i=0; i<0x7e - 0x20 + 4; i++)
    {
        for(char j=0; j<0x7e - 0x20 + 4; j++)
        {
            add_transition(states_inside_bracket_comments + i, states_inside_bracket_comments + j);
        }
    }
    STATE* whitespace_states = (STATE*) malloc(num_whitespace * sizeof(STATE));
    for(size_t i=0; i<num_whitespace; i++)
    {
        STATE state = { whitespace[i], NULL, 0, 0, T_ID };
        memcpy(whitespace_states + i, &state, sizeof(STATE));
    }
    for(size_t i=0; i<num_whitespace; i++)
    {
        for(size_t j=0; j<num_whitespace; j++)
        {
            add_transition(whitespace_states + i, whitespace_states + j);
        }
        add_transition(whitespace_states + i, start_comment_brace);
        add_transition(whitespace_states + i, maybe_start_comment_bracket);
        add_transition(end_comment_brace, whitespace_states + i);
        add_transition(end_comment_bracket, whitespace_states + i);
    }

    for(size_t i=0; i<num_whitespace; i++)
    {
        add_transition(sm->initial_state, whitespace_states + i);
    }
    add_transition(sm->initial_state, start_comment_brace);
    add_transition(sm->initial_state, maybe_start_comment_bracket);

    return sm;
}
