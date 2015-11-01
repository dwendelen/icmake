/*
        Symboltable structure:


        After a functions is defined, the field

            g_funtab.symbol[index_of_the_function].var.vu.i->count

        contains the offset of the 1st byte of the function in the g_bin file.


    Adding a hidden (macro) function:
    =================================

    ../RSS/ICRSS.H:
        1.  add a f_... functionname before the f_hlt entry in the FUNNR enum.
    PARSER:
        1.  define a parser constant, to be returned by the lexer
        2.  expand the proper nonterminal activated in the 'function'
            nonterminal, to recognize the new parser constant
        3.  adapt the corresponding C function to recognize the new f_-entry.
            This function should call 'callhidden()'
    LEXER:
        1.  add the name of the function to the set of keywords, return the
            newly defined parser constant, set the yylval.type field to the
            newly defined f_-value.
    DATA.C:
        1.  add the definition of the function to hidden[]
        2.  add the name of the function to funstring[]
    ICCOMP.H (this file):
        1.  add an entry he_... to the HIDDEN_ENUM enum, before the final he_
            entry
    touch
            CLEARHID.C and YYLEXHID.C
    adapt
            DOC/ICMAKE.1, copy this file to \sys\man, and update
            \sys\man\icmake.1
    remake
            icm-comp.

*/

#include "../rss/icrssdef.h"

#include <memory.h>
#define bcopy(s,d,l)    memcpy((d),(s),(l))

typedef enum                                /* order of elements must follow */
{                                           /* definition of hidden[] in     */
    he_older = 0,                           /* data.c                        */
    he_younger = 1,                         /* reserved values 0 and 1 */

    he_                                     /* must be last ! */
} HIDDEN_ENUM_;

typedef enum
{
    pre_op,
    post_op
} PREPOST_;

typedef enum
{
    j_uncond,                               /* unconditional jump */
    j_truelist,                             /* jump batchpatch for truelist */
    j_falselist,                            /* jump backbatch for falselist */
    j_continuelist                          /* jump backp. for continuelist */
} JMP_COND_;

#define down_type(e,v)          ((e)->type &= ~(v))
#define test_type(e,v)          ((e)->type &   (v))
#define set_type(e,v)           ((e)->type =   (v))
#define up_type(e,v)            ((e)->type |=  (v))

#define ALLTYPES                (e_int | e_list | e_str | e_bool)

#define codestruc(estruc, x)    (&(((ESTRUC_ *)((estruc)->code))[x]))

typedef struct
{
    unsigned index;                         /* index in stringsection */
    char *string;                           /* string itself */
} STRINGTAB_;

typedef struct                              /* symtab used with the compiler */
{
    VAR_ var;
    char *name;
} SYMBOL_;

typedef struct
{
    unsigned
        n_allocated,                        /* available memory */
        n_defined;                          /* defined variables */
    SYMBOL_
        *symbol;
} SYMTAB_;

typedef struct                              /* see also display code in */
{                                           /* callfun.c                */
    E_TYPE_
        type;                               /* type of the expression */
    size_t
        truelen,
        falselen,
        continuelen,
        codelen,                            /* length of the code */
        evalue;                             /* index or value of the expression */
    unsigned
        *truelist,
        *falselist,
        *continuelist;
    INT8
        *code;
} ESTRUC_;

typedef struct
{
    char
        *name,                              /* name of the hidden fun */
        *source;                            /* source of the hidden function */
    unsigned
        type,                               /* returntype */
        this,                               /* set to 1 if called */
        nargs;                              /* # of arguments */
} HIDDEN_FUNCTION_;

#define YYSTYPE ESTRUC_

/*
    Prototypes of often used functions and variabels defined in code
    generated by FLEX and BISON.

*/

#ifndef YYSTYPE
    #define YYSTYPE int
#endif

extern YYSTYPE
    yylval;                                 /* yyparse() semantic value */

extern FILE
    *yyin,                                  /* yylex() input file */
    *yyout;                                 /* yylex() output file */

extern int
    initialization,                         /* for initialization expr. */
    yy_init;                                /* yylex() initializer: 1 to init. */

extern size_t yyleng;                       /* strlen(yytext) */

extern int yynerrs;                         /* number of parse errors so far */

extern int yylineno;                        /* yylex() line counter */

extern char *yytext;                        /* yylex() read input chars */

extern int
    parse_error,
    (*yylex_input)(char *, int);            /* pointer to yylex input to use */

int yyerror(char *);                        /* yyparse() error function */
int yylex(void);                            /* yylex() */
int yyparse(void);                          /* yyparse() */



typedef enum
{
    err_openpar_expected,
    err_closepar_expected,
    err_openbrace_expected,
    err_closebrace_expected,
    err_semicol_expected,
    err_assign_expected,
    err_in_expression,
    err_comma_expected,
    err_statements_expected,
    err_identifier_expected,
    err_code_or_vars_expected,
    err_comma_or_closepar_expected,
    err_number_expected,
    err_older_younger,
    err_backtick_expected
} PARSE_ERR_;

extern char
    release[],
    *g_filenames,
    *funstring[],
    icm_comp[],
    illegal_argcount[],
    illegal_cast[],
    illegal_type[],
    init_expr_not_const[],
    lvalue_needed[],
    not_on_lists[],
    not_og_nStrings[],
    g_nullstring[],
    only_on_ints[],
    *opstring[],
    *g_sourceName,
    *g_lexstring,
    *g_stringbuf,
    type_conflict[],
    version[];

extern E_TYPE_
    vartype;

extern E_TYPE_
    gp_opType[];


extern FILE
    *g_bin;

extern OPCODE_
    lastop;

extern STRINGTAB_
    *g_stringtab;

extern SYMTAB_
    g_funtab,
    local,
    g_globaltab,
    *g_entertab;

extern HIDDEN_FUNCTION_
    hidden[he_];

extern size_t
    break_ok,
    g_dead_sp,
    g_errcount,
    g_hidden_called,
    nestlevel,
    n_params,
    g_nStrings,
    g_sem_err,
    stringsize;

unsigned *g_dead;

extern ESTRUC_ g_init;             /* code for initializing globals */

int conflict (ESTRUC_ *, ESTRUC_ *,     /* conflicting binary types */
                  OPCODE_);
int test_binop (OPCODE_, ESTRUC_ *,     /* test binop legality */
                      ESTRUC_ *);
int test_operand (ESTRUC_ *, OPCODE_);  /* test legality of operand */
int yylex_file(char *, int);            /* read yylex input from yyin */
int yylex_hidden(char *, int);          /* read yylex input from buffer */

size_t fetchfun (void);              /* fetch index of function */
size_t lookstring (char *);          /* look for string in g_stringtab */
size_t looksym (SYMTAB_ *);          /* look for symbol in symboltab */
size_t rm_jmp_zero (size_t,         /* remove jmp 0 from || && lists */
                 unsigned *, size_t);

ESTRUC_ *assignconst(ESTRUC_ *, ESTRUC_ *);     /* initialization code */
ESTRUC_ *assignment  (ESTRUC_ *, ESTRUC_ *,     /*  = code */
                                    char *);
                             
ESTRUC_ *icast (ESTRUC_ *);                /* cast to int */

ESTRUC_ *insertarg (ESTRUC_ *, ESTRUC_ *);/* arg1, before arg2, ... */
ESTRUC_ *lcast (ESTRUC_ *);                /* cast to list */

ESTRUC_ *nullframe(ESTRUC_ *e);              /* discard(e) + *e = stackfr(0) */

ESTRUC_ *scast (ESTRUC_ *);                /* cast to str */

ESTRUC_ *strupr_lwr (E_TYPE_, ESTRUC_ *);       /* strupr<->strlwr */

void    addpatch (unsigned *, size_t,     /* add value to patch-list */
                               size_t);
void    backend (void);                /* finish g_bin construction */
void    btoi (ESTRUC_ *);              /* boolean to int */
void    callrss (ESTRUC_ *, FUNNR_,     /* call rss function */
                                ...);   /* and add asp, xxx instruction */
void    callhidden(int, ESTRUC_ *);         /* call hidden function */
void    catargs (ESTRUC_ *);           /* arguments to code */
void    catstrings (ESTRUC_ *,          /* catenate string consts */
                             ESTRUC_ *);
void    change_file (char *);          /* switch to other file */
void    clearbin (ESTRUC_ *,            /* clear and init 2 ESTRUC_s */
                      ESTRUC_ *);
void    clear_hidden(void);                 /* clear hidden function-names */
void    copy_to_pop (ESTRUC_ *);       /* op_copy_var to op_pop_var */
void    defcode (ESTRUC_ *, ESTRUC_ *,  /* generate default e_int|e_code */
                  OPCODE_);
void    discard (ESTRUC_ *);           /* free memory used by ESTRUC_ */
void    etob (ESTRUC_ *);              /* expr. prepare for boolean */
void    etoc (ESTRUC_ *);              /* convert E to code */
void    enter (void);                  /* enter somthing in a symtab */
void    entervar (void);               /* enter variable in l/g-tab */
void    fetob (ESTRUC_ *);             /* forced e conversion to boolean */
void    hidden_functions (void);            /* patchup/generate hidden funs */
void    last_stmnt (ESTRUC_ *);        /* write last stmnt */
void    gencode (ESTRUC_ *, OPCODE_,    /* append new code */
                                   ...);
void    open_fun (void);               /* open a function */

void    outcode (ESTRUC_ *, int,        /* append code to e->code */
                        size_t);
void    patchcontinue(ESTRUC_ *);      /* jmp_continue target */
void    patchfalse (ESTRUC_ *);        /* jmp_false target */
void    patchtrue (ESTRUC_ *);         /* jmp_true target */
void    patchup (INT8 *, size_t,      /* patchup t/f list */
                     unsigned *, size_t, int);
void    patchup_true (ESTRUC_ *, int); /* batchpatch truelist */
void    patchup_false (ESTRUC_ *, int);/* batchpatch truelist */
void    patchup_continue(ESTRUC_ *e, int pos);  /* backpatch continuelist */

void    semantic (char *, ...);         /* give semantic error */

