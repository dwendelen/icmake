/*
\funcref{fun\_arg\_tail}{void fun\_arg\_tail ()}
    {}
    {}
    {eb_setString()}
    {fun\_arg\_tail(), fun\_cmd\_tail()}
    {funargte.c}
    {

        This function is called when an {\em op\_arg\_tail} opcode is read in
        the binary makefile. The {\em gb_argTail} variable is set to a duplicate
        of the string in the last pushed variable. The last pushed variable is
        then discarded.

    }
*/

#include "builtin.ih"

void b_argTail()
{
    gb_argTail = eb_setString(gb_argTail);
}
