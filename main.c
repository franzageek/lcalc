#include "src/parse.h"
#include "src/reduction.h"

#include <stdio.h>
#include <intdef.h>
#include <colors.h>
#include <string.h>
#include <stdlib.h>
#include <linenoise.h>
#include <evec.h>

int main(/*int argc, char** argv*/)
{
    printf("-- λ-expression parser, v0.1 --\n");
    linenoiseHistorySetMaxLen(10);
    while (true)
    {
        char* line = linenoise("["C_GREEN"λ"C_RESET"] >>> ");
        //& char* line = "(((\\x.\\y.\\z.\\x.\\y.\\z.x \\x.\\y.\\z.x) \\x.\\y.x) ())";
        if (line[0] == '\0')
            continue;
        
        if (strcmp(line, "quit") == 0)
            break;

        u16 u = 0;
        while (line[u++] == ' ');
        --u;
        if (line[u] == '\0')
            continue;
        

        linenoiseHistoryAdd(&line[u]);
        printf("\n> %s\n", &line[u]);
        term_t* t = parse(&line[u]);
        term_t* a = t;
        if (t) 
        {
            printf(C_RGB_FG(0, 100, 200)"<start of tree-based expression analysis>\n"C_RESET);
            do
            {
                term__print(a, 0);
                ++a;
            }
            while (a->type != null);
            printf(C_RGB_FG(0, 100, 200)"<end of tree-based expression analysis>\n\n"C_RESET);

            printf(C_RGB_FG(0, 100, 200)"<start of beta-reduction>"C_RESET"\n");
            char* expr = NULL;
            while (beta_reduce(t, false))
            {

                /*if (beta_reduce(t, true))
                    printf("expression was reduced\n");*/ // this is for single step beta-reduction
    

                /* all that follows is needed to merge each one of the resulting terms
                 * into a single NULL-terminated string of lambda terms.
                 */

                if (expr)
                    free(expr);

                a = t;
                evec_t* strs = evec__new(sizeof(char*));           
                printf("-->");
                while (a->type != null)
                {
                    char* str = term__to_string(a);
                    printf(" %s", str);
                    evec__push(strs, &str);
                    ++a;
                }
                printf("\n");
    
                size_t exprlen = 0;
    
                u = 0;
                while (u < strs->size)
                {
                    exprlen += strlen(*((char**)evec__at(strs, u++)));
                    exprlen += 1; // for spaces and NULL terminator 
                }
                exprlen += 2; //some breathing room for the last term to be written
    
                expr = calloc(exprlen, sizeof(char));
                if (!expr)
                {
                    perror("calloc:");
                    exit(1);
                
                }
                exprlen -= 2; // we'll pretend we have got the same space as before, but in case strcat() needs those extra bytes (for spaces or N-Ts) they are there
    
                u = 0;
                while (u < strs->size)
                {
                    strcat(expr, *((char**)evec__at(strs, u)));
                    free(*((char**)evec__at(strs, u++)));
                    strcat(expr, " ");
                }
                expr[exprlen-1] = '\0'; // set the N-T where we expect it to be
                
                printf("+-> %s\n", expr);
                evec__free(strs);
            }
            if (!expr)
                printf("the expression has already been reduced to normal form\n");

            printf(C_RGB_FG(0, 100, 200)"<end of beta-reduction>"C_RESET"\n");

            if (!expr)
                printf("-*> %s\n", line);
 
            else
            {
                printf("-*> %s\n", expr);
                linenoiseHistoryAdd(expr);
                free(expr);
            }
            a = t;
            while (a->type != null)
            {
                term__free_sub(a);
                ++a;
            }
            free(t);
        }
        putc('\n', stdout);
        linenoiseFree(line);
    }
    exit(0);
}