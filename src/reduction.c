#include "reduction.h"

#include <stdlib.h>
#include <colors.h>
#include <evec.h>


/*

[ ] alfa conversion - rec-traverse a tree of abstracions and replace every occurrence of * with an alternative
[ ] beta reduction - for each application, take the argument and replace it with every occurrence of the bound variable

*/

term_t* replace_param(char param, term_t* term, term_t* repl)
{
    if (!repl)
        return NULL;

    switch (term->type)
    {
        case variable:
        {
            if (term->variable == param)
                return repl;

            break;
        }

        case abstraction:
        {
            term_t* val = replace_param(param, term->abstraction.body, repl);
            if (val == repl)
            {
                free(term->abstraction.body);
                term->abstraction.body = repl;
                return term;
            }
            if (val == term->abstraction.body)
            {
                /*if (term->abstraction.param == param)
                    return term->abstraction.body;*/
             
                return term;
            }
            return NULL;
        }

        default:
            return NULL;
    }
    return NULL;
}

bool substitute(term_t* term)
{
    if (term->type == application)
    {
        if (term->application.funct->type == abstraction)
        {
            term_t* val = replace_param(term->application.funct->abstraction.param, term->application.funct, term->application.arg);    
            if (val->type != null)
            {
                memmove(term, val->abstraction.body, sizeof(term_t));
                term__free(val); // like this it DOESNT LEAK, but when freeing "val" while its *body is pointing at a term that has subterms, those subterms are freed too, therefore the newly copied term has dead pointers
                // %p
                return true;
            }
        }
    }
    return false;
}

void beta_reduce(term_t* terms)
{
    if (!terms)
        return;

    //evec_t* evec = evec__new(sizeof(term_t));    
    term_t* t = terms;
    while (t->type != null)
    {
        if (t->type == application)
        {
            if (substitute(t))
            {
                term__print(t, 0);
            }
        }
        //evec__push(evec, t);
        ++t;
    }
    /*term_t n = {0};
    evec__push(evec, &n);
    term_t* r = (term_t*)evec->data;
    free(evec);*/
    printf(C_RGB_FG(0, 100, 200)"<end of beta-reduction>"C_RESET"\n");
    return /*r*/;
}