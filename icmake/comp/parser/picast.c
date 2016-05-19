#include "parser.ih"

SemVal *p_iCast(SemVal *e)
{
    if (test_type(e, e_list))               /* (int)list not ok */
    {
        util_semantic(gp_illegalCast);
        p_discard(e);
        set_type(e, e_int | e_const);
    }
    else if (test_type(e, e_str))           /* (int)string ok */
    {
        if (test_type(e, e_const))          /* string const to a */
        {
            e->evalue =                      /* convert to string */
                atoi(gp_stringTable[e->evalue].string);
            set_type(e, e_const | e_int);
        }
        else
        {
            p_expr2stack(e);                        /* convert to code */
            p_generateCode(e, op_atoi);            /* runtime conversion needed */
            set_type(e, e_int | e_stack);
        }
    }
    return e;
}
