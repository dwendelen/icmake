/*
    Frame organization:

            Low address
                        Local x     <- Var 0xbffc   = 0xbfff - x
                        Local ...   <- Var 0xbffd   = 0xbfff - ...
                        Local 1     <- Var 0xbffe   = 0xbfff - 1
                        Local 0     <- Var 0xbfff   = 0xbfff - 0
                        Old BP      <- Var 0xc000
                        RA          <- Var 0xc001
            parameters: Par 0       <- Var 0xc002   = 0xc002 + 0
                        Par 1       <- Var 0xc003   = 0xc002 + 1
                        ...
            High Address
*/

#include "parser.ih"

static SemVal ret;

SemVal *fetchvar()
{
    register size_t idx;

    ret = *stackframe(e_null);

    VarIndex vi = symtab_findVar();      /* find the index of var.
                                               util_string() */

    if (vi.idx == -1)                          
    {
        util_semantic("%s undefined", util_string());
        return &ret;
    }

//FBB fetchvar must distinguish between global vars, params, and local vars:

    size_t nParams = 0;

    if (vi.idx < nParams)                   /* idx refers to a parameter */
        idx += 0xc002;
    else                                    /* idx refers to a local var */
        vi.idx = 0xbfff - (vi.idx - nParams); 

    ret.evalue = vi.idx;
    ret.type = symtab_varType(vi);

//                                            /* not a local variable ? */
//    if ((idx = looksym(&gp_local)) == gp_local.n_defined)
//    {                                       /* not a global variable ? */
//        if ((idx = looksym(&g_globaltab)) != g_globaltab.n_defined)
//            type = g_globaltab.symbol[idx].var.type;
//        else
//        {
//            idx = 0xffff;
//            util_semantic("%s undefined", util_string());
//        }
//    }
//    else
//    {
//        type = gp_local.symbol[idx].var.type;
//        if (idx < gp_nParams)               /* Parameters: */
//            idx += 0xc002;
//        else                                /* Locals: */
//            idx = 0xbfff - (idx - gp_nParams);
//    }
//
//    if (idx != 0xffff)
//    {
//        ret.evalue = idx;                /* set idx and type */
//        ret.type =  type;
//    }

    return &ret;                         /* return the frame */
}
