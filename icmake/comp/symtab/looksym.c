#include "symtab.ih"

size_t looksym(SYMTAB_ *tab)
{
    register size_t idx;

    for (idx = 0; idx != tab->n_defined; ++idx)
        if (!strcmp(tab->symbol[idx].name, g_lexstring))/* string found ? */
            break;                                      /* done */

    return idx;                                 /* return string idx */
}