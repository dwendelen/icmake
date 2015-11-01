#include "parser.ih"

int conflict(ESTRUC_ *lval, ESTRUC_ *rval, OPCODE_ opcode) 
{
    register int ret;

    if ( (ret = !(lval->type & rval->type & gp_opType[opcode])) )
    {
        semantic(gp_typeConflict, gp_opstring[opcode]);
        clearbin(lval, rval);
    }
    return ret;
}
