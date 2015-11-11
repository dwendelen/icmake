/*
                            F I R S T A R G . C
*/

#include "parser.ih"

static SemVal
    arr;

SemVal *firstarg(SemVal *e)
{
    eToStack(e);                                /* argument is code now */
                                            /* code points to SemVal */
    arr.code = rss_realloc(NULL, sizeof(SemVal));

    *(SemVal *)arr.code = *e;              /* arr->code contains e */
    arr.type = 1;                           /* type field: # of active e's */
    return (&arr);
}
