/*
    This function is called when an immediate value is to be pushed. The
    value (type {\em int16_t}) is expected to follow the opcode.

    A variable of type {\em e\_int}, with its {\em vu.intval} field set
    to the value retrieved from the binary makefile, is pushed.
*/

#include "opcodefun.ih"

void fun_push_imm ()
{
    IntVariable tmp = *intConstructor_i(rss_getInt16(infile));

    push(&tmp);
    intDestructor(&tmp);
}

