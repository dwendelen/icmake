#ifndef INCLUDED_FILESTACK_H_
#define INCLUDED_FILESTACK_H_

#include <stdio.h>

typedef struct              /* yylex itself maintains its own YY_BUFFERs */
{
    char    *filename;      /* name of the currently active file */
    FILE    *file;
    int     popLineNr;      /* value of yylineno when popping this element */
} FileStack;


void filestack_push(unsigned currentLineNr, char const *fname);
void filestack_pop();
FileStack const *filestack_tos();
unsigned filestack_size();

#endif
