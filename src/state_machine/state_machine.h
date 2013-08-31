#ifndef __STATE_MACHINE_H
#define __STATE_MACHINE_H

#include "state_machine/state_machine.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct STATE
{
    const char state;
    struct STATE** allowed_transitions; // Do not alter this directly. always use add_transition
    size_t num_transitions; // Same as before.
    size_t _allocated_mem; // Do not use outside of add_transition. You shouldn't ever need to access it.
    const int return_token;
} STATE;

typedef struct STATE_MACHINE
{
    STATE* current_state;
    STATE* const initial_state;
} STATE_MACHINE;

void
add_transition(STATE* first,
          STATE* second);
int
execute_transition(STATE_MACHINE* sm,
                   char c);

void
reset_machine(STATE_MACHINE* sm);


#endif // __STATE_MACHINE_H
