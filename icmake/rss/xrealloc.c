/*
\funcref{xrealloc}{VOIDP xrealloc (\params)}
    {
        {VOIDP} {ptr} {pointer to previously allocated memory, or NULL}
        {int} {size} {new requested size, or 0}
    }
    {pointer to reallocated memory}
    {error()}
    {xstrdup()}
    {xrealloc.c}
    {
        {\em xrealloc()} attempts to reallocate the memory pointed to by {\em
        ptr}. If {\em ptr} is NULL, {\em xrealloc()} simply behaves like {\em
        malloc()}. When allocation indicates failure, {\em error()} is called
        to terminate the program with an appropriate message.

        The new requested size may be zero. In this case, {\em xrealloc()}
        frees the memory associated with {\em ptr}.
    }
*/


#include <malloc.h>
#include "icrssdef.h"

#ifdef MSDOS

void *xrealloc (void *ptr, int size)
{
    register void
        *newptr;

    if (! size)
    {
        if (ptr)
            free (ptr);
        return (NULL);
    }

    newptr = malloc (size);

    if (! newptr)
        error ("out of memory");

    if (ptr)
    {
        memcpy (newptr, ptr, size);
        free (ptr);
    }

    return (newptr);
}

#else

void *xrealloc (void *ptr, int size)
{
    register void
        *newptr;

    if (! size)
    {
        if (ptr)
            free (ptr);
        return (NULL);
    }

    if (ptr)
        newptr = realloc (ptr, size);
    else
        newptr = malloc (size);

    if (! newptr)
        error ("out of memory");

    return (newptr);
}

#endif
