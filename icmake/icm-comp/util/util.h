#ifndef INCLUDED_UTIL_H_
#define INCLUDED_UTIL_H_

#include <stdio.h>

char const *util_sourceName(void);
char const *util_string(void);

int         util_printf(char const *fmt, ...);  /* fill gu_lexstring */

void        util_catString(char const *txt);
void        util_out(FILE *bin, void const *source, unsigned sourceSizeInBytes);
void        util_resetSemErr(void);
void        util_semantic(char const *s, ...);
void        util_setSourceName(char const *txt);
void        util_setString(char const *txt);
void        util_scan(char const *fmt, char const *text);

#endif


