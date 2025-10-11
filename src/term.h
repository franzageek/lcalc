#ifndef TERM_H
#define TERM_H

#include <intdef.h>

typedef enum term_type
{
    variable = 0,
    abstraction = 1,
    application = 2
} term_type;

#define str_term_type(a) a == 0 ? "type::variable" : a == 1 ? "type::abstraction" : "type::application"

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
void term__free(term_t* term);

#endif //TERM.H