#include "builtin.ih"

/* 
    enter the first of a series of characters, terminated by \n. The first
    char may be a \n
*/

int eb_enterCh()
{
    int ch = getchar();
    if (ch != '\n')
    {
        while (getchar() != '\n')
            ;
    }
    return ch;
}
