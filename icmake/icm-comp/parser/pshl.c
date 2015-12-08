#include "parser.ih"

SemVal *p_shl(SemVal *lval, SemVal *rval)
{
    if (p_testBinOp(op_shl, lval, rval))
        return p_nullFrame(lval, rval);     /* test for correct types */

    p_bool2int(lval);                       /* convert pending booleans */
    p_bool2int(rval);

    if (p_conflict(lval, rval, op_shl))     /* test type p_conflict */
        return p_nullFrame(lval, rval);   


    if ((lval->type & rval->type & (unsigned)~e_typeMask) == e_const)
        lval->evalue <<= rval->evalue;
    else
        p_binOp(lval, rval, op_shl);

    return lval;                            /* return new expression */
}
