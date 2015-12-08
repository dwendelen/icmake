/*
    This function is called when a string constant is to be pushed onto the
    stack. The offset of the string to be pushed, relative to the strings
    section of the binary makefile, is expected to follow the opcode. This
    relative offset is an {\em uint16_t} variable.

    A variable of type {\em e\_str} and with field {\em valuestr} set to
    a duplicate of the string constant is pushed.
*/

/* #define msg
*/

#include "opcodefun.ih"

void o_push_strconst()
{
    register char *str = rss_getString(go_infile, go_header->offset[0], 
                                           (unsigned)rss_getInt16(go_infile));
    
    if (str == (char *)-1)
        rss_fatal(0, 0, "cannot get string, opcode at %s", aux_offset());

                                        /* str is allocated by getstring() */
    String tmp;
    stringcons_charPtr(&tmp, str);
    stack_push(&tmp);
    stringDestructor(&tmp);

    free(str);
}




