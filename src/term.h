#ifndef TERM_H
#define TERM_H

#include <intdef.h>

typedef enum term_type
{
    null = 0,
    variable = 1,
    abstraction = 2,
    application = 3
} term_type;

#define str_term_type(a) a == 0 ? " null " : a == 1 ? " variable " : a == 2 ? " abstraction " : " application "

typedef struct term_t
{
    term_type type;
    union
    {
        char variable;
        struct
        {
            char param;
            struct term_t* body;
        } abstraction;
        struct
        {
            struct term_t* funct;
            struct term_t* arg;
        } application;
    };
} term_t;

void term__print(term_t* term, u16 level);
void term__print_raw(term_t* term, u16 level);
char* term__to_string(term_t* term);
term_t* term__duplicate(term_t* term);
void term__free_sub(term_t* term);
void term__free(term_t* term);

#endif //TERM.H