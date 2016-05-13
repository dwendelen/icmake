#include "dependencies.ih"

void d_inspectIH(int idx, char const *ihFile)
{
    char *ih = rss_strdup(ihFile);

    vmsg(1, "Inspect %s\n", ih);

    Vector *toDo = VectorCons();
    add(toDo, ih);

    assign(s_Dependencies.gchPaths, idx,    // create gch file path names
                    strcatN(4, depDir(idx), "/", ih, ".gch"));

//    d_statGch(idx);                         // try to stat the file

    d_checkUseAll(idx);

    int next = 0;
    while (next < vSize(toDo))              // as long as there are files to
    {                                       // inspect included headers,
        d_inspectFile(idx, toDo, next);     // and possibly .gch files
        ++next;
    }

    VectorDestructor(toDo);
    free(ih);
}


