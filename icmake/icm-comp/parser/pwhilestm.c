#include "parser.ih"

SemVal *p_while(SemVal *expr, SemVal *stmnt, int pureWhile)
{
    register size_t len;
    unsigned *list;

    gp_nestLevel--;                     /* reduce nesting level */
    gp_breakOK--;                       /* reduce break ok */

    p_expr2bool(expr);                  /* make links for EXPR */

    if (test_type(expr, e_const))       /* constant: never xeq */
    {
        if (expr->evalue)
            expr->evalue = 0;           /* no value = no code for p_catCode */
                                        /* MAYBE CODELEN = 0 ?? */
        else
        {
            p_clearOperands(&expr, stmnt);
            return expr;
        }
    }

    p_patchupTrue(expr, 1);             /* patch to EOC */

    list = expr->falselist;
    len  = expr->falselen;

    expr->falselen = 0;                 /* no more false links avail. */

    p_catCode(expr, stmnt);             /* append stmnt to expr */

    if (pureWhile)                      /* not a while stmd that's part */
        p_patchupContinue(expr, -expr->codelen);   /* of a for stmnt    */

    p_generateCode(expr, op_jmp, j_falselist);  /* jmp to begin of code */

    p_patchupFalse(expr, 0);            /* patch to BOC */

    expr->falselist = list;
    expr->falselen = len;

    p_patchupTrue(expr, 1);            /* p_patchup break targets to EOC */

    return expr;
}





