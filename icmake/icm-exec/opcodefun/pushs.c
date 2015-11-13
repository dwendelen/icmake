/*
\funcref{fun\_push\_strconst} {void fun\_push\_strconst ()}
    {}
    {}
    {rss_getInt16(), rss_getString(), rss_error(), push()}
    {}
    {funpushs.c}
    {

        This function is called when a string constant is to be pushed onto the
        stack. The offset of the string to be pushed, relative to the strings
        section of the binary makefile, is expected to follow the opcode. This
        relative offset is an {\em uint16_t} variable.

        A variable of type {\em e\_str} and with field {\em vu.str} set to
        a duplicate of the string constant is pushed.

    }
*/

#include "opcodefun.ih"

void fun_push_strconst()
{
    register char *str = rss_getString(infile, headerp->offset[0], 
                                           (size_t)rss_getInt16(infile));
    
    if (str == (char *)-1)
        rss_error ("cannot get string, opcode at %s", rss_hexString (curoffs, 4));

    {
                                        /* str is allocated by getstring() */
        String tmp = *stringConstructor_cP(str);
        push(&tmp);
        stringDestructor(&tmp);
        free(str);
    }
}
