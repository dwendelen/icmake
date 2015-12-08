#include "parser.ih"

void p_catArgs(SemVal *arr)
{
    register unsigned count;
    SemVal *ep;
    SemVal e;

    if (!(count = arr->type))
        return;                             /* no arguments */

    ep = (SemVal *)arr->code;              /* local pointer to SemVals */

    e = ep[--count];                        /* e: code of last argument */

    while (count--)
        p_catCode(&e, &ep[count]);            /* catenate next argument(s) */

    free(arr->code);                         /* memory of array is free again */
    *arr = e;                               /* arguments changed to code */
}
