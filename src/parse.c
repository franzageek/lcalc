#include "term.h"

#include <intdef.h>
#include <colors.h>
#include <evec.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_syntax_error(u16 index)
{
    while (index)
    {
        fprintf(stderr, " ");
        --index;
    }
    fprintf(stderr, C_BRED"  ^"C_RESET"\n");
    fprintf(stderr, C_BRED"syntax error"C_RESET": ");
    return;
}

term_t* parse_term(char* expr, u16* index)
{
    if (!expr)
        return NULL;

    while (expr[*index] != '\0')
    {
        switch (expr[*index]) {
            case '\\':
            {
                // [ ] add currying
                ++*index;
                if (expr[*index] < 'a' || expr[*index] > 'z')
                {
                    if (expr[*index] == '\0')
                    {
                        print_syntax_error(*index);
                        fprintf(stderr, "missing parameter name\n");
                        return NULL;
                    }

                    print_syntax_error(*index);
                    fprintf(stderr, "invalid parameter name: \'%c\'\n", expr[*index]);
                    return NULL;
                }
                char param = expr[*index];
                ++*index;
                if (expr[*index] != '.')
                {
                    print_syntax_error(*index);
                    fprintf(stderr, "\'.\' expected\n");
                    return NULL;
                }
                ++*index;
                if (expr[*index] == '\0' || expr[*index] == ' ')
                {
                    print_syntax_error(*index);
                    fprintf(stderr, "function body expected\n");
                    return NULL;
                }
                term_t* body = parse_term(expr, index);
                if (!body)
                    return NULL;

                term_t* term = calloc(1, sizeof(term_t));
                if (!term)
                {
                    term__free(body);
                    return NULL;
                }

                term->type = abstraction;
                term->abstraction.param = param;
                term->abstraction.body = body;

                return term;
            }
            
            case '(':
            {
                ++*index;
                term_t* funct = parse_term(expr, index);
                if (!funct)
                    return NULL;
                
                if (expr[*index] == '\0' || expr[*index] == ')')
                {
                    print_syntax_error(*index);
                    fprintf(stderr, "application argument is missing\n");
                    term__free(funct);
                    return NULL;
                }

                if (expr[*index] != ' ')
                {   
                    print_syntax_error(*index);
                    fprintf(stderr, "space expected between function and argument\n");
                    term__free(funct);
                    return NULL;
                }
                ++*index;

                if (expr[*index] == '\0' || expr[*index] == ')')
                {
                    print_syntax_error(*index);
                    fprintf(stderr, "application argument is missing\n");
                    term__free(funct);
                    return NULL;
                }

                term_t* arg = parse_term(expr, index);
                if (!arg)
                {
                    term__free(funct);
                    return NULL;
                }

                if (expr[*index] != ')')
                {
                    print_syntax_error(*index);
                    fprintf(stderr, "\')\' expected\n");
                    term__free(funct);
                    term__free(arg);
                    return NULL;
                }
                ++*index;
                term_t* term = calloc(1, sizeof(term_t));
                if (!term)
                {
                    term__free(funct);
                    term__free(arg);
                    return NULL;
                }

                term->type = application;
                term->application.funct = funct;
                term->application.arg = arg;
                return term;
            }
            default:
            {
                if (expr[*index] >= 'a' && expr[*index] <= 'z')
                {
                    term_t* term = calloc(1, sizeof(term_t));
                    if (!term)
                        return NULL;

                    term->type = variable;
                    term->variable = expr[*index];
                    ++*index;
                    return term;
                }
                else if (expr[*index] != ' ') 
                {
                    print_syntax_error(*index);
                    fprintf(stderr, "unknown template\n");
                    return NULL;
                }
            }
        }
        ++*index;
    }
    print_syntax_error(*index);
    fprintf(stderr, "incomplete expression (NUL reached)\n");
    return NULL;
}

term_t* parse(char* expr)
{
    evec_t* evec = evec__new(sizeof(term_t));
    u16 i = 0;
    while (expr[i] != '\0')
    {
        if (expr[i] == ' ')
        {
            ++i;
            continue;
        }

        term_t* t = parse_term(expr, &i);
        if (!t)
        {
            u16 u = 0;                          
            while (u < evec->size)
                term__free_sub(((term_t*)evec__at(evec, u++))); // free all subterms of each term inside the evec

            evec__free(evec);
            return NULL;
        }
        
        evec__push(evec, t); // [x] temporary setup, do stable
        free(t); // copy term to evec, preserve children, free term
    }
    term_t n = {0};
    evec__push(evec, &n); // push null term to evec
    term_t* t = (term_t*)evec->data;
    free(evec); // drop evec, keep NULL-terminated term array
    return t;
}