#ifndef __SYMREC_H
#  define __SYMREC_H

typedef struct SYMREC
{
    size_t id;
    int type;
    union
    {
        int int_const;
        float real_const;
    };
    struct SYMREC* next;
} SYMREC;

/*
 Returns the first record found in the scope of record
*/
SYMREC* find_in_scope(SYMREC* scope_head, size_t id);

/*
 Adds record to head's tail.
*/
void add(SYMREC** head, SYMREC* record);

/*
 Searches for id in any of tail's superscopes, starting on
 tail, and decrementing until it reaches head.

 To illustrate: This will run through the scope nodes in
 this order:
 ___________
|           |
|___________|           ___________ 
|     0     |<-- tail  |     1     |
|___________|--------->|___________|           ___________
|     2     |          |     3     |          |     4     |
|___________|--------->|___________|--------->|___________|
|     5     |
|___________|
|     6     |<-- head
|___________|

*/
SYMREC* find(SYMREC** tail, SYMREC** head, size_t id);

#endif
