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
        if (line[0] == '\0')
            continue;
        
        if (strcmp(line, "quit") == 0)
            return 0;

        printf("> %s\n", line);
        linenoiseHistoryAdd(line);
        /*term_t* t =*/ parse(line);
        /*if (t)
        {
            print_term(t, 0);
            free(t);
        }*/
        linenoiseFree(line);
    }
    return 0;
}