/*
    This function reads in a string from a file and returns it in the {\em
    gb_reg} return register as an {\em e\_str} value. The arguments on the
    stack are: the filename and an {\em int offset}, where the read
    operation should start.

    The return value is a list, holding as the first list the just read line,
    as the second element the final \n (if available), as its third
    element the string OK, or FAIL, and as its fourth
    b_element the (long) offset value from where the next string must be
    read. This long value is stored as binary value using the host byte order.

    On EOF an empty list is returned.
*/

#include "builtin.ih"

void b_fgets()
{
    char const *filename = string_charp(stack_top());
    Variable *list = stack_top() - 1;
    FILE *inf;

    listcons(eb_releaseReg());

    char const *status;
                                                /* input not OK */
    if (list_size(list) >= 4 && strcmp(status = list_at(list, 2), "OK") != 0)
    {
        eb_fgetsStatus(status);
        return;
    }

    long offset = list_size(list) == 0 ? 
                        0 
                    : 
                        strtol(list_at(list, 3), 0, 10);
    if 
    (                                       
        (inf = fopen(filename, "r"))
        &&                                      
        (fseek(inf, offset, SEEK_SET) == 0)     /* locate the read-pos. */
    )
    {
        char *dest = eb_getLine(inf);
        
        if (dest)                               /* anything read?           */
            eb_updateFgets(inf, dest);
        else if (!feof(inf))                    /* no, and not EOF: fail    */
            eb_fgetsStatus("FAIL");

        fclose (inf);
        return;                                 /* empty list at EOF        */
    }

    eb_fgetsStatus("FAIL");
}
