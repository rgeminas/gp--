#include "token.h"
#include <stdlib.h>

#ifndef __STATE_MACHINE_H
#define __STATE_MACHINE_H

typedef struct STATE
{
    const char state;
    STATE** allowed_transitions; // Do not alter this directly. always use add_transition
    size_t num_transitions; // Same as before.
    size_t _allocated_mem; // Do not use outside of add_transition. You shouldn't ever need to access it.
    const TOKEN_TYPE return_token;
} STATE;

typedef struct STATE_MACHINE
{
    STATE* current_state;
    STATE* const initial_state;
} STATE_MACHINE;

void
add_transition(STATE* first, 
          STATE* second)
{
    // We increase the buffer size that we want.
    ++first->num_transitions;

    // If we don't have any memory, give us some.
    if(first->_allocated_mem == 0)
    {
        ++first->_allocated_mem;
        first->allowed_transitions = (STATE**) malloc(sizeof(STATE*));
    }
    // If we don't have enough memory, give us double the memory we have now!
    // The 2x figure is a speed tradeoff. The amount of reallocs needed is O(log(n)),
    // the space used is still O(n), and you never waste more memory than you are
    // already using.
    if(first->num_transitions > first->_allocated_mem)
    {
        first->_allocated_mem = first->_allocated_mem << 1;
        first->allowed_transitions = (STATE**) realloc(first->allowed_transitions, first->_allocated_mem * sizeof(STATE*));
    }
    first->allowed_transitions[first->num_transitions - 1] = second;
}

/*
Sets sm's current_state to one of its allowed transitions.
*/
int
execute_transition(STATE_MACHINE* sm, 
                   char c)
{
    for(size_t i=0; i < sm->current_state->num_transitions; i++)
    {
        // We jump to the first transition whose state matches the input.
        if(sm->current_state->allowed_transitions[i]->state == c)
        {
            sm->current_state = sm->current_state->allowed_transitions[i];
            return EXIT_SUCCESS;
        }
    }
    // No legal transition.
    return EXIT_FAILURE;
}

/*
Sets sm to its initial state.
*/
void
reset_machine(STATE_MACHINE* sm)
{
    sm->current_state = sm->initial_state;
}
#endif // __STATE_MACHINE_H