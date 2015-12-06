#include "parser.ih"

SemVal *p_and(SemVal *lval, SemVal *rval)
{
    if (p_testBinOp(op_band, &lval, rval))
        return lval;                        /* test for correct types */

    p_bool2int(lval);                       /* convert pending booleans */
    p_bool2int(rval);

    if (p_conflict(&lval, rval, op_band))   /* test type p_conflict */
        return lval;

    if ((lval->type & rval->type & (size_t)~e_typeMask) == e_const)
        lval->evalue &= rval->evalue;
    else
        p_binOp(lval, rval, op_band);

    return lval;                            /* return new expression */
}
