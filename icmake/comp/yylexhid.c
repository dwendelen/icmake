/*
                                Y Y L E X H I D . C

                   Function called when yylex_file() is at eof.

    If new hidden functions are defined, then the array hidden[] must be
    enlarged to contain the name of the new function.
*/

#include "iccomp.h"

static int
    hidden_index = -1;                      /* hidden function index */

static char
    *cp = nullstring;

int yylex_hidden(char *buf, register int max_size)
{
    register int
        result = 0;

    while (max_size)                        /* fill as much as possible */
    {
        if (!*cp)                           /* test available source */
        {
            if                              /* test if next index will point */
            (                               /* to another hidden function */
                hidden_index
                ==
                sizeof(hidden) / sizeof(HIDDEN_FUNCTION_) - 1
            )
                break;                      /* if not: done, return 'result' */

            hidden_index++;                 /* next index and next source */


            if (!hidden[hidden_index].this)
                continue;                   /* if not called, no code */

            cp = hidden[hidden_index].source;
        }

        result++;                           /* count a char for the return */
        max_size--;                         /* one char filled */
        *buf++ = *cp++;                     /* and copy the char           */
    }

    return (result);                        /* number of chars processed   */
}
