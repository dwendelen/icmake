#include "parser.ih"

ESTRUC_ *young(ESTRUC_ *lval, ESTRUC_ *rval)
{
    if (test_binop(op_younger, lval, rval))
        return lval;                        /* test for correct types */

    etoc(lval);                             /* convert to code */
    etoc(rval);

    defcode(lval, rval, op_younger);

    return lval;                            /* return new expression */
}