#include <stdio.h>
#include <intdef.h>
#include <string.h>
#include <linenoise.h>

int main(int argc, char** argv)
{
    linenoiseHistorySetMaxLen(10);
    printf("-- λ-expression parser, v0.1 --\n");
    while (true)
    {
        char* line = linenoise("[λ] >>> ");
        if (line[0] == '\0')
            continue;
        
        if (strcmp(line, "quit") == 0)
            return 0;

        printf("> %s\n", line);
        linenoiseHistoryAdd(line);
        linenoiseFree(line);
    }
    return 0;
}