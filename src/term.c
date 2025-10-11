#include "term.h"

#include <colors.h>
#include <stdlib.h>
#include <stdio.h>

void term__print(term_t* term, u16 level)
{
    level *= 2;
    u16 i = 0;
    switch (term->type)
    {
        case variable:
        {
            for (i = level; i > 0; --i)
                putc(' ', stdout);
            printf(C_BOLD"type: "C_RESET C_BG_MAGENTA"%s"C_RESET"\n", str_term_type(variable));
            for (i = level; i > 0; --i)
                putc(' ', stdout);
            printf(C_BOLD"variable: "C_RESET C_BMAGENTA"%c"C_RESET"\n", term->variable);
            return;
        }

        case abstraction:
        {
            for (i = level; i > 0; --i)
                putc(' ', stdout);
            printf(C_BOLD"type: "C_RESET C_BG_CYAN"%s"C_RESET"\n", str_term_type(abstraction));
            for (i = level; i > 0; --i)
                putc(' ', stdout);
            printf(C_BOLD"bound variable/parameter: "C_RESET C_BCYAN"%c"C_RESET"\n", term->abstraction.param);
            for (i = level; i > 0; --i)
                putc(' ', stdout);
            printf(C_BOLD"body:"C_RESET" \n");
            term__print(term->abstraction.body, level/2+1);
            return;
        }
    
        case application:
        {
            for (i = level; i > 0; --i)
                putc(' ', stdout);
            printf(C_BOLD"type: "C_RESET C_BG_RED"%s"C_RESET"\n", str_term_type(application));
            for (i = level; i > 0; --i)
                putc(' ', stdout);
            printf(C_BOLD C_BRED"f"C_RESET C_BOLD"unction:"C_RESET" \n");
            term__print(term->application.funct, level/2+1);
            for (i = level; i > 0; --i)
                putc(' ', stdout);
            printf(C_BOLD C_BGREEN"a"C_RESET C_BOLD"rgument:"C_RESET" \n");
            term__print(term->application.arg, level/2+1);
            return;
        }
        default:
            return;
    }
}

void term__free(term_t* term)
{
    switch (term->type)
    {
        case variable:
        {
            free(term);
            return;
        }

        case abstraction:
        {
            term__free(term->abstraction.body);
            free(term);
            return;
        }

        case application:
        {
            term__free(term->application.funct);
            term__free(term->application.arg);
            free(term);
            return;
        }
    }
}