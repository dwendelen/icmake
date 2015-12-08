#include "symtab.ih"

unsigned st_newVarIdx()   /* return the index of a (unitialized) var. record */
{
    VarTab *vt = st_varTab(st_nestingOffset());

    if (vt->nVars == vt->capacity)
        vt->info = rss_realloc(vt->info, (vt->capacity += capacityInc) 
                                      * sizeof(VarInfo));

    return vt->nVars++;
}
