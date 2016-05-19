/*
\funcref{fun\_arg\_head}{void fun\_arg\_head ()}
    {}
    {}
    {eb_setString()}
    {fun\_arg\_tail(), fun\_cmd\_tail()}
    {funarghe.c}
    {

        This function is called when an {\em op\_arg\_head} opcode is read in
        the binary makefile. The {\em gb_argHead} variable is set to a duplicate
        of the string in the last pushed variable. The last pushed variable is
        then discarded.

    }
*/

#include "builtin.ih"

void b_argHead ()
{
    gb_argHead = eb_setString (gb_argHead);
}
