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

static ESTRUC_ ret;

ESTRUC_ *fetchvar()
{
    register size_t
        idx;
    E_TYPE_
        type = 0;

    ret = *stackframe(0);

    if (gp_initialization)
    {
        semantic(gp_init_expr_not_const);
        return &ret;
    }
                                            /* not a local variable ? */
    if ((idx = looksym(&gp_local)) == gp_local.n_defined)
    {                                       /* not a global variable ? */
        if ((idx = looksym(&g_globaltab)) == g_globaltab.n_defined)
        {
            idx = 0xffff;
            semantic("%s undefined", g_lexstring);
        }
        else
            type = g_globaltab.symbol[idx].var.type;
    }
    else
    {
        type = gp_local.symbol[idx].var.type;
        if (idx < gp_nParams)               /* Parameters: */
            idx += 0xc002;
        else                                /* Locals: */
            idx = 0xbfff - (idx - gp_nParams);
    }

    if (idx != 0xffff)
    {
        ret.evalue = idx;                /* set idx and type */
        ret.type =  type;
    }
    return &ret;                         /* return the frame */
}