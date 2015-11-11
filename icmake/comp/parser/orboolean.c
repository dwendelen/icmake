#include "parser.ih"

SemVal *or_boolean(SemVal *lexp, SemVal *rexp)
{
    if (lexp->type & rexp->type & e_const)  /* two constants: compute result */
    {
        lexp->evalue =
            (test_type(lexp, e_str) || lexp->evalue)
            ||
            (test_type(rexp, e_str) || rexp->evalue);
        set_type(lexp, e_const | e_int);
    }
    else                                    /* at least one code-part */
    {
        fetob(lexp);                        /* boolean code */
        fetob(rexp);

        lexp->codelen -= rm_jmp_zero(lexp->codelen, lexp->falselist,
                                    lexp->falselen);
        patchup_false(lexp, 1);
        lexp = catcode(lexp, rexp);
        set_type(lexp, e_bool | e_stack);
    }

    return (lexp);
}
