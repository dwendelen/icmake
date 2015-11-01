/*
                             B N O T . C
*/

#include "parser.ih"

ESTRUC_ *bnot(ESTRUC_ *e)                   /* expression so far */
{
    if (test_operand(e, op_bnot))            /* test types ok */
    {
        semantic(gp_illegalType, gp_opstring[op_bnot]);
        return e;
    }

    if ((e->type & (size_t)~ALLTYPES) == e_const)   /* immediate value */
        e->evalue = ~e->evalue;
    else
    {
        etoc(e);                            /* convert to code */
        gencode(e, op_bnot);                /* generate instruction */
    }

    return e;
}