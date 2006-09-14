/*
                            S M E Q U A L . C
*/

#include "iccomp.h"

ESTRUC_ *sm_equal (lval, rval)
    ESTRUC_
        *lval,
        *rval;
{
    btoi(lval);                             /* convert boolean to i */
    btoi(rval);

    if (conflict(lval, rval, op_smeq))      /* test type conflict */
        return(lval);

    if ((lval->type & rval->type & ~ALLTYPES) == e_const)
    {
        if (test_type(lval, e_int))
            lval->evalue = (lval->evalue <= rval->evalue);
        else
        {
            lval->evalue =
                      (
                        strcmp
                        (
                            stringtab[lval->evalue].string,
                            stringtab[rval->evalue].string
                        )
                      ) <= 0;
            set_type(lval, e_int | e_const);
        }
    }
    else if (initialization)
    {
        semantic(init_expr_not_const);
        return nullframe(lval);
    }

    defcode(lval, rval, op_smeq);

    return (lval);                          /* return new expression */
}
