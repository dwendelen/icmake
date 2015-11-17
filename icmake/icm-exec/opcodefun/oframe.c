/*
    This function is executed when an {\em op\_frame} opcode is
    encountered. Following this opcode, the number of local variables is
    expected. When room for local variables should be made, a series of
    variable types is expected.

    The function imitates the assembler opcodes {\em push bp; mov bp, sp}
    by pushing an {\em e\_int} variable with the value of {\em bp} as {\em
    vu.intval} field. If local variables should be created, then {\em
    newvar()} is called to create a variable (the variable type is read
    from the binary makefile) and {\em push()} is called to make room for
    the variable.
*/

#include "opcodefun.ih"

void o_frame()
{
    size_t nlocals = rss_getOpcode(go_infile);
    size_t idx;

    for (idx = 0; idx != nlocals; ++idx)
    {
        Variable frame = *constructor((ExprType)rss_getOpcode(go_infile));
        stack_push(&frame);
        destructor(&frame);
    }
}

