#include "icmun.h"

void fun_jmp_false ()
{
    INT16
        offs;

    offs = getint16 (infile);
    dumpint ( (UNS16) offs);
    printf ("jmp false [%s]\n",
            hexstring ((int) (ftell (infile) + offs), 4));
}
