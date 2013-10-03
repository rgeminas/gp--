/*
 Searches for id in any of tail's superscopes, starting on
 tail, and decrementing until it reaches head.

 To illustrate: This will run through the scope nodes in
 this order:
 ___________
|           |
|___________|                ___________ 
|     0     |<-- head->head |     1     |
|___________|-------------->|___________|           ___________
|     2     |               |     3     |          |     4     |
|___________|-------------->|___________|--------->|___________|
|     5     |
|___________|
|     6     |<-- first->base
|___________|

*/
SYMREC* 
find_in_scope(SCOPE* head,
     SCOPE* first,
     size_t id);
