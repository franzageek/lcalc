#include "reduction.h"

#include <stdlib.h>
#include <colors.h>
#include <evec.h>
#include <string.h>


/*

[ ] alfa conversion - rec-traverse a tree of abstracions and replace every occurrence of * with an alternative
[ ] beta reduction - for each application, take the argument and replace it with every occurrence of the bound variable

*/

/*void unhook_subterms(term_t* term)
{
    switch (term->type)
    {
        case null:
        case variable:
            return;
        
        case abstraction:
        {
            term->abstraction.body = nullterm;
            return;
        }

        case application:
        {
            term->application.funct = nullterm;
            term->application.arg = nullterm;
        } 
        
        default:
            return;
    }
    return;
}*/

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
                free(term->abstraction.body); //it is a variable
                term->abstraction.body = repl;
                return term;
            }
            if (val == term->abstraction.body)
            {
                /*if (repl == val->abstraction.body)
                {

                }*/
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
            term_t* val = term->application.funct;
            if (replace_param(term->application.funct->abstraction.param, term->application.funct, term->application.arg) == NULL)
                term__free(term->application.arg);

            memmove(term, term->application.funct->abstraction.body, sizeof(term_t));
            memset(val->abstraction.body, 0, sizeof(term_t));
            //unhook_subterms(val->abstraction.body);
            term__free(val); //[x] fixed leak and memory layout
            return true;
        }
        else if (term->application.funct->type == application)
        {
            return substitute(term->application.funct);
        }
    }
    return false;
}

bool beta_reduce(term_t* terms)
{
    printf(C_RGB_FG(0, 100, 200)"<start of beta-reduction>"C_RESET"\n");
    bool reduction = false;
    if (!terms)
        return reduction;

    term_t* t = terms;
    while (t->type != null)
    {
        if (substitute(t))
        {
            reduction = true;
            term__print(t, 0);
        }
        ++t;
    }
    printf(C_RGB_FG(0, 100, 200)"<end of beta-reduction>"C_RESET"\n");
    return reduction;
}