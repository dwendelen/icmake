#include "dependencies.ih"

void d_fillTable()
{
                                                // inspect all class-ih files
    for (int idx = 1, end = sdep.size; idx != end; ++idx)
    {
        chdir(depDir(idx));
        d_inspectIH(idx, d_ihFile(idx));
        toCwd();
    }
    d_inspectIH(0, optMainih());
}
