#include "parser.ih"

SemVal *p_oneArg(ExprType type, SemVal *arg)
{
    msg("calling function 0x%x", type);

    register int ok;

    p_expr2stack(arg);                               /* arg to stack */

    switch ((FunNr)type)
    {
        case f_ascii_int:
            if ( (ok = test_type(arg, e_str | e_int)) )
            {
                if (test_type(arg, e_int))  /* int received    */
                    type = f_ascii_str;     /* string returned */
            }

        break;

        case f_listlen:
            if (strcmp(scanner_savedText(), "listlen") != 0)
                rss_warning(util_sourceName(), 
                            scanner_savedLineNr(), 
                            "`%s' is deprecated. Use `listlen'\n", 
                            scanner_savedText());

            ok = test_type(arg, e_list);
        break;

        case f_echo:
            ok = test_type(arg, e_int);
        break;

        default:
        /*
            case f_backtick:
            case f_getenv:
            case f_putenv:
            case f_eval:
            case f_exists:
            case f_cmd_head:
            case f_cmd_tail:
            case f_arg_head:
            case f_arg_tail:
            case f_g_path:
            case f_g_base:
            case f_g_ext:
            case f_strlen:
            case f_strlwr:
            case f_strupr:
       		case f_trim:
       		case f_trimright:
       		case f_trimleft:
       */
            ok = test_type(arg, e_str);
    }

    if (ok)
        p_callRss(arg, type);
    else
        util_semantic(gp_typeConflict, gp_funstring[type]);

    return (arg);
}
