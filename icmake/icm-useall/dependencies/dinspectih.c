#include "dependencies.ih"

void d_inspectIH(int idx, char const *ihFile)
{
    char *ih = rss_strdup(ihFile);

    vmsg(1, "Inspect %s\n", ih);

    Vector *toDo = VectorCons();
    add(toDo, ih);

    sdep.currentGch = rss_strdup(strcatN(2, ih, ".gch"));

    assign(sdep.gchPaths, idx,    // create gch file path names
                    strcatN(3, depDir(idx), "/", 
                            sdep.currentGch));

    d_checkUseAll(idx);

    int next = 0;
    while (next < vSize(toDo))              // as long as there are files to
    {                                       // inspect included headers,
        d_inspectFile(idx, toDo, next);     // and possibly .gch files
        ++next;
    }

    VectorDestructor(toDo);
    free(sdep.currentGch);
    free(ih);
}


