#include "icmake.ih"

int spawnlp(int mode, char const *prog, char const *av0, ...)
{
    va_list marker;
    register char *nextarg;
    char buf[MAX_PATHLEN * 4];

    strcpy(buf, prog);

    va_start(marker, av0);
    nextarg = va_arg (marker, char*);

    while (nextarg)
    {
        strcat(buf, " ");
        strcat(buf, nextarg);
        nextarg = va_arg(marker, char*);
    }

    return system(buf);
}

#ifdef DEBUG
int main ()
{
    _spawnlp (0, "ls", "ls", "*.c", "*.h", NULL);

    return (0);
}
#endif

