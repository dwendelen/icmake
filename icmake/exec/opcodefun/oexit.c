/*
    This function places the {\em int} value of the return register {\em
    reg} into the global variable {\em retval}.
*/

#include "opcodefun.ih"

void o_exit()
{
    go_retVal = int_value(builtin_regPtr());
}
