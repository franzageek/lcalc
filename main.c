#include "src/parse.h"

#include <stdio.h>
#include <intdef.h>
#include <colors.h>
#include <string.h>
#include <stdlib.h>
#include <linenoise.h>

int main(/*int argc, char** argv*/)
{
    linenoiseHistorySetMaxLen(10);
    printf("-- λ-expression parser, v0.1 --\n");
    while (true)
    {
        char* line = linenoise("["C_GREEN"λ"C_RESET"] >>> ");
        //& char* line = "(((\\x.\\y.\\z.\\x.\\y.\\z.x \\x.\\y.\\z.x) \\x.\\y.x) ())";
        if (line[0] == '\0')
            continue;
        
        if (strcmp(line, "quit") == 0)
            return 0;

        linenoiseHistoryAdd(line);
        printf("\n> %s\n", line);
        term_t* t = parse(line);
        term_t* a = t;
        if (t) 
        {
            do
            {
                term__print(a, 0);
                term__free_sub(a); // free any children this term may have, preserve term array (will be freed later with free())
                printf("--\n");
                ++a;
            }
            while (a->type != null);
            free(t);
        }
        putc('\n', stdout);
        linenoiseFree(line);
    }
    return 0;
}