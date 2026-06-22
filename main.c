#include "src/parse.h"
#include "src/reduction.h"

#include <stdio.h>
#include <intdef.h>
#include <colors.h>
#include <string.h>
#include <stdlib.h>
#include <linenoise.h>

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
            do
            {
                term__print(a, 0);
                ++a;
            }
            while (a->type != null);
            if (beta_reduce(t))
                printf("expression was reduced\n");

            a = t;
            while (a->type != null)
            {
                char* str = term__to_string(a);
                linenoiseHistoryAdd(str);
                printf("--> %s\n", str);
                free(str);
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