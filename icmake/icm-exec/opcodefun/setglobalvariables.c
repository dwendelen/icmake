/* #define msg
*/

#include "opcodefun.ih"

void opcodefun_setGlobalVariables()
{
    unsigned nvars = rss_getVar(&go_globalVar, go_infile, go_header);

    msg("N global vars: %u", nvars);

                                        /* return array of global vars */
    if (nvars == (int16_t)-1)
        rss_fatal(0, 0, "invalid bim-file, cannot read variable section");

        /* global string/list variables haven't been initialized by */
        /*  the compiler, so that's icm-exec's job                  */
    for (Variable *var = go_globalVar, *end = var + nvars; var != end; ++var)
    {
        switch (var_type(var))
        {
            case e_str:
                stringcons(var);
            break;

            case e_list:
                listcons(var);
            break;

            default:
            // not handled
            break;
        }
    }

    fseek(go_infile, go_header->offset[3], SEEK_SET);
}





