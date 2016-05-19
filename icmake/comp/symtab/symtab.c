#include "symtab.ih"

/* 
    Initializes the symbol table structure. Call only once.
*/
    
void symtab()   
{
    gs_vars.varTab = rss_realloc(NULL, 2 * sizeof(VarTab));

    memset(gs_vars.varTab, 0, sizeof(VarTab)); /* the global table */
    memset(gs_vars.varTab + 1, 0, sizeof(VarTab)); /* the local tab. */
    
    gs_vars.nLevels = 1;
    gs_vars.capacity = capacityInc;
    gs_vars.varIndices = rss_realloc(NULL, sizeof(VarIndices) * capacityInc);
    memset(gs_vars.varIndices, 0, sizeof(VarIndices)); /* var indices in the 
                                                            global table */
}
