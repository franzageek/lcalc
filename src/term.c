#include "term.h"

#include <colors.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void term__print_raw(term_t* term, u16 level)
{
    level *= 2;
    u16 i = 0;
    for (i = level; i > 0; --i)
        putc(' ', stdout);
    printf(C_BOLD"type: "C_RESET C_BG_BWHITE C_BLACK"%s"C_RESET C_BBLACK" (0x%02X)\n"C_RESET, str_term_type(term->type), term->type);
    for (i = level; i > 0; --i)
        putc(' ', stdout);
    printf(C_BOLD"variable: "C_RESET C_BBLACK"%c"C_RESET" / "C_BBLACK"0x%02X"C_RESET"\n", term->variable, term->variable);
    for (i = level; i > 0; --i)
        putc(' ', stdout);
    printf(C_BOLD"abstraction: "C_RESET C_BBLACK"%c"C_RESET" / "C_BBLACK"0x%02X"C_RESET" -> "C_BBLACK"%p"C_RESET"\n", term->abstraction.param, term->abstraction.param, (void*)term->abstraction.body);
    for (i = level; i > 0; --i)
        putc(' ', stdout);
    printf(C_BOLD"application: "C_RESET C_BBLACK"%p"C_RESET" -> "C_BBLACK"%p"C_RESET"\n", (void*)term->application.funct, (void*)term->application.arg);
    return;
}

void print_spaces(u16 level, bool print_number)
{
    u16 i = 0;
    for (i = level; i > 0; --i)
        putc(' ', stdout);

    if (print_number)
        printf(C_BBLACK"%02d "C_RESET, level/2);

    return;
}

void term__print(term_t* term, u16 level)
{
    level *= 2;
    switch (term->type)
    {
        case null:
        {
            print_spaces(level, true);
            printf(C_BOLD"type: "C_RESET C_BG_BBLACK"%s"C_RESET"\n", str_term_type(null));
            return;
        }

        case variable:
        {
            print_spaces(level, true);
            printf(C_BOLD"type: "C_RESET C_BG_MAGENTA"%s"C_RESET"\n", str_term_type(variable));
            print_spaces(level, true);
            printf(C_BOLD"variable: "C_RESET C_BMAGENTA"%c"C_RESET"\n", term->variable);
            return;
        }

        case abstraction:
        {
            print_spaces(level, true);
            printf(C_BOLD"type: "C_RESET C_BG_CYAN"%s"C_RESET"\n", str_term_type(abstraction));
            print_spaces(level, true);
            printf(C_BOLD"bound variable/parameter: "C_RESET C_BCYAN"%c"C_RESET"\n", term->abstraction.param);
            print_spaces(level, true);
            printf(C_BOLD"body:"C_RESET" \n");
            term__print(term->abstraction.body, level/2+1);
            return;
        }
    
        case application:
        {
            print_spaces(level, true);
            printf(C_BOLD"type: "C_RESET C_BG_RED"%s"C_RESET"\n", str_term_type(application));
            print_spaces(level, true);
            printf(C_BOLD C_BRED"f"C_RESET C_BOLD"unction:"C_RESET" \n");
            term__print(term->application.funct, level/2+1);
            print_spaces(level, true);
            printf(C_BOLD C_BGREEN"a"C_RESET C_BOLD"rgument:"C_RESET" \n");
            term__print(term->application.arg, level/2+1);
            return;
        }
        default:
            return;
    }
}

char* term__to_string(term_t* term)
{
    switch (term->type)
    {
        case null:
        {
            return strdup("<nul>");
        }

        case variable:
        {
            char str[2] = {0};
            sprintf(str, "%c", term->variable);
            return strdup(str);
        }

        case abstraction:
        {
            char* body = term__to_string(term->abstraction.body);
            size_t strsize = strlen("\\x.") + strlen(body);
            char* str = calloc(strsize+1, sizeof(char));
            sprintf(str, "\\%c.%s", term->abstraction.param, body);
            free(body);
            return str;
        }

        case application:
        {
            char* funct = term__to_string(term->application.funct);
            char* arg = term__to_string(term->application.arg);
            size_t strsize = strlen("( )") + strlen(funct) + strlen(arg);
            char* str = calloc(strsize+1, sizeof(char));
            sprintf(str, "(%s %s)", funct, arg);
            free(funct);
            free(arg);
            return str;
        }

        default:
            return NULL;
    }
}

term_t* term__duplicate(term_t *term)
{
    if (!term)
    {
        //should never happen since we're in control of what goes in here,
        //easier than handling the error all the way up the call stack.
        fprintf(stderr, "lcalc: term__duplicate(): invalid term provided\n");
        exit(1);
    }

    switch (term->type)
    {
        case null:
        case variable:
        {
            term_t* dup = malloc(sizeof(term_t));
            if (!dup)
            {
                perror("malloc:");
                exit(1);
            }
            memcpy(dup, term, sizeof(term_t));
            return dup;
        }

        case abstraction:
        {
            term_t* dup_body = term__duplicate(term->abstraction.body);
            term_t* dup = malloc(sizeof(term_t));
            if (!dup)
            {
                perror("malloc:");
                exit(1);
            }
            dup->type = abstraction;
            dup->abstraction.param = term->abstraction.param;
            dup->abstraction.body = dup_body;
            return dup;
        }

        case application:
        {
            term_t* dup_funct = term__duplicate(term->application.funct);
            term_t* dup_arg = term__duplicate(term->application.arg);
            term_t* dup = malloc(sizeof(term_t));
            if (!dup)
            {
                perror("malloc:");
                exit(1);
            }
            dup->type = application;
            dup->application.funct = dup_funct;
            dup->application.arg = dup_arg;
            return dup;
        }
    }
    return NULL; //technically unreachable
}

void term__free_sub(term_t* term)
{
    if (!term)
        return;

    switch (term->type)
    {
        case null:
        case variable:
            return;

        case abstraction:
        {
            term__free(term->abstraction.body);
            return;
        }

        case application:
        {
            term__free(term->application.funct);
            term__free(term->application.arg);
            return;
        }
    }
}

void term__free(term_t* term)
{
    term__free_sub(term);
    free(term);
        
    return;
}