#include "opcodefun.ih"

void opcodefun_setInfile(char const *arg)
{
    if (!(go_infile = fopen(arg, "r")))
        rss_error("cannot read bimfile '%s'", arg);

    go_header = rss_readHeader(go_infile, (size_t)version[0]);
}