/*
    This function serves the evaluation of logical expressions. It is
    executed when an {\em op\_push\_0} opcode is found in the binary
    makefile. A variable of type {\em e\_int} is pushed, while its {\em
    vu.intval} field is set to zero.
*/

#include "opcodefun.ih"

void o_push_0()
{
    IntVariable const *tmp = intcons_int(0);
    stack_push(tmp);
    intDestructor(tmp);
}
