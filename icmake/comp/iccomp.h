/*
        Symboltable structure:


        After a functions is defined, the field

            funtab.symbol[index_of_the_function].var.vu.i->count

        contains the offset of the 1st byte of the function in the s_bin file.


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

#ifdef MSDOS
#define __STDC__
#endif


#ifndef MSDOS
#include <memory.h>
#define bcopy(s,d,l)    memcpy((d),(s),(l))
#endif

typedef enum                                /* order of elements must follow */
{                                           /* definition of hidden[] in     */
    he_older = 0,                           /* data.c                        */
    he_younger = 1,                         /* reserved values 0 and 1 */
    he_strlwr,                              /* strlwr macro */
    he_strupr,                              /* strupr macro */
    he_strlen,                              /* strlen macro */
    he_substr,                              /* substr macro */

    he_,                                    /* must be last ! */
} HIDDEN_ENUM_;

typedef enum
{
    pre_op,
    post_op,
} PREPOST_;

typedef enum
{
    j_uncond,                               /* unconditional jump */
    j_truelist,                             /* jump batchpatch for truelist */
    j_falselist,                            /* jump backbatch for falselist */
} JMP_COND_;

#define down_type(e,v)          ((e)->type &= ~(v))
#define test_type(e,v)          ((e)->type &   (v))
#define set_type(e,v)           ((e)->type =   (v))
#define up_type(e,v)            ((e)->type |=  (v))

#define ALLTYPES                (e_int | e_list | e_str | e_bool)

#define codestruc(estruc, x)    (&(((ESTRUC_ *)((estruc)->code))[x]))

typedef struct
{
    unsigned
        index;                              /* index in stringsection */
    char
        *string;                            /* string itself */
} STRINGTAB_;

typedef struct                              /* symtab used with the compiler */
{
    VAR_
        var;
    char
        *name;
} SYMBOL_;

typedef struct
{
    unsigned
        n_allocated,                        /* available memory */
        n_defined;                          /* defined variables */
    SYMBOL_
        *symbol;
} SYMTAB_;

typedef struct
{
    E_TYPE_
        type;                               /* type of the expression */
    unsigned
        truelen,
        falselen,
        codelen,                            /* length of the code */
        evalue,                             /* index or value of the expression */
        *truelist,
        *falselist;
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
                             B I F L E X

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
    yy_init,                                /* yylex() initializer: 1 to init. */
    yyleng,                                 /* strlen(yytext) */
    yynerrs;                                 /* number of parse errors so far */

extern int
    yylineno;                               /* yylex() line counter */

extern char
    *yytext;                                /* yylex() read input chars */

extern int
    parse_error,
    (*yylex_input)(char *, int);            /* pointer to yylex input to use */

int yyerror(char *);                        /* yyparse() error function */
int yylex(void);                            /* yylex() */
int yyparse(void);                          /* yyparse() */

/*
                               B I F L E X

                            E N D S    H E R E
*/

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
} PARSE_ERR_;

extern char
    release[],
    *filenames,
    *funstring[],
    icm_comp[],
    illegal_argcount[],
    illegal_cast[],
    illegal_type[],
    init_expr_not_const[],
    lvalue_needed[],
    not_on_lists[],
    not_on_strings[],
    nullstring[],
    only_on_ints[],
    *opstring[],
    *source_name,
    string[],
    *stringbuf,
    type_conflict[],
    version[];

extern E_TYPE_
    vartype;

extern E_TYPE_
    optype[];


extern FILE
    *s_bin;

extern OPCODE_
    lastop;

extern STRINGTAB_
    *stringtab;

extern SYMTAB_
    funtab,
    local,
    global,
    *entertab;

extern HIDDEN_FUNCTION_
    hidden[he_];

extern unsigned
    break_ok,
    *dead,
    dead_sp,
    errcount,
    hidden_called,
    nestlevel,
    n_params,
    n_strings,
    sem_err,
    stringsize;


int conflict (ESTRUC_ *, ESTRUC_ *,     /* conflicting binary types */
                  OPCODE_);
int test_binop (OPCODE_, ESTRUC_ *,     /* test binop legality */
                      ESTRUC_ *);
int test_operand (ESTRUC_ *, OPCODE_); /* test legality of operand */
int yylex_file(char *, int);                /* read yylex input from yyin */
int yylex_hidden(char *, int);              /* read yylex input from buffer */

unsigned fetchfun (void);              /* fetch index of function */
unsigned lookstring (char *);          /* look for string in stringtab */
unsigned looksym (SYMTAB_ *);          /* look for symbol in symboltab */
unsigned rm_jmp_zero (unsigned,         /* remove jmp 0 from || && lists */
                 unsigned *, unsigned);

ESTRUC_ *addition (ESTRUC_ *, ESTRUC_ *);  /* + code */
ESTRUC_ *and_boolean (ESTRUC_ *, ESTRUC_ *);/* && code */
ESTRUC_ *assign  (ESTRUC_ *, ESTRUC_ *);        /*  = code (shell) */
ESTRUC_ *assignconst(ESTRUC_ *, ESTRUC_ *);     /* initialization code */
ESTRUC_ *assignment  (ESTRUC_ *, ESTRUC_ *,     /*  = code */
                                    char *);
ESTRUC_ *band (ESTRUC_ *, ESTRUC_ *);           /* & (binary) code */
ESTRUC_ *bnot (ESTRUC_ *);                      /* ~ code */
ESTRUC_ *bor  (ESTRUC_ *, ESTRUC_ *);           /* | (binary) code */
ESTRUC_ *break_stmnt (void);           /* process break stmnt */
ESTRUC_ *callfun (unsigned, ESTRUC_ *);  /* call function */
ESTRUC_ *cast (E_TYPE_, ESTRUC_ *);      /* perform cast */
ESTRUC_ *catcode (ESTRUC_ *,              /* write info rval behind lval */
                             ESTRUC_ *);
ESTRUC_ *cat_expr (ESTRUC_ *,               /* ,-separated expressions */
                             ESTRUC_ *);
ESTRUC_ *cat_stmnt (ESTRUC_ *,              /* catenate/write stmnts */
                             ESTRUC_ *);
ESTRUC_ *divide (ESTRUC_ *, ESTRUC_ *);    /* / code */
ESTRUC_ *equal (ESTRUC_ *, ESTRUC_ *);     /* == code */
ESTRUC_ *exec_fprintf (E_TYPE_, ESTRUC_ *);/* exec() and fprintf() */
ESTRUC_ *execute (ESTRUC_ *);              /* execute() (full arglist) */
ESTRUC_ *expr_stmnt (ESTRUC_ *);           /* expr ; code */
ESTRUC_ fetchvar (void);                   /* fetch variable */
ESTRUC_ *firstarg (ESTRUC_ *);             /* (arg   code */
ESTRUC_ *first_stmnt (ESTRUC_ *);          /* catenate/write stmnts */
ESTRUC_ *for_stmnt (ESTRUC_ *, ESTRUC_ *,   /* for statement */
                       ESTRUC_ *, ESTRUC_ *);
ESTRUC_ *greater (ESTRUC_ *, ESTRUC_ *);   /* > code */
ESTRUC_ *gr_equal (ESTRUC_ *, ESTRUC_ *);  /* >= code */
ESTRUC_ *icast (ESTRUC_ *);                /* cast to int */
ESTRUC_ *if_stmnt (ESTRUC_ *, ESTRUC_ *,    /* if code */
                               ESTRUC_ *);
ESTRUC_ *incdec (PREPOST_, OPCODE_,         /* E.g., c++ */
                               ESTRUC_ *);
ESTRUC_ *insertarg (ESTRUC_ *, ESTRUC_ *);/* arg1, before arg2, ... */
ESTRUC_ *lcast (ESTRUC_ *);                /* cast to list */
ESTRUC_ *makelist (ESTRUC_ *, E_TYPE_);         /* makelist() */
ESTRUC_ *math_ass (ESTRUC_ *, ESTRUC_ *,    /* shell for math-asgnmt */
                       ESTRUC_ *(*)(ESTRUC_ *, ESTRUC_ *),
                       char *);
ESTRUC_ *modulo (ESTRUC_ *, ESTRUC_ *);    /* % code */
ESTRUC_ *multargs (ESTRUC_ *, ESTRUC_ *);  /* (arg1, arg2, ... code */
ESTRUC_ *multiply (ESTRUC_ *, ESTRUC_ *);  /* * code */
ESTRUC_ *negate (ESTRUC_ *);               /* - (unary) code */
ESTRUC_ *not_boolean (ESTRUC_ *);          /* ! code */
ESTRUC_ *nullframe(ESTRUC_ *e);              /* discard(e) + *e = stackfr(0) */
ESTRUC_ *old (ESTRUC_ *, ESTRUC_ *);       /* older code */
ESTRUC_ *onearg (E_TYPE_, ESTRUC_ *);      /* fun(x)  code */
ESTRUC_ *optint_special (E_TYPE_,           /* fun([int,] ...) */
                      ESTRUC_ *, ESTRUC_ *);
ESTRUC_ *optint_string (E_TYPE_,        /* chdir(), system() */
                      ESTRUC_ *, ESTRUC_ *);
ESTRUC_ *or_boolean (ESTRUC_ *, ESTRUC_ *);/* || code */
ESTRUC_ *return_stmnt (E_TYPE_, ESTRUC_ *);/* exit(), return(); */
ESTRUC_ *specials (E_TYPE_, ESTRUC_ *);    /* fun(x, y, ...)  code */
ESTRUC_ *scast (ESTRUC_ *);                /* cast to str */
ESTRUC_ *shl (ESTRUC_ *, ESTRUC_ *);            /* << (binary) code */
ESTRUC_ *shr (ESTRUC_ *, ESTRUC_ *);            /* >> (binary) code */
ESTRUC_ *smaller (ESTRUC_ *, ESTRUC_ *);   /* < code */
ESTRUC_ *sm_equal (ESTRUC_ *, ESTRUC_ *);  /* <= code */
ESTRUC_ stackframe (E_TYPE_);              /* initialize a stack-element */
ESTRUC_ *subtract (ESTRUC_ *, ESTRUC_ *);  /* - (binary) code */
ESTRUC_ *strupr_lwr (E_TYPE_, ESTRUC_ *);       /* strupr<->strlwr */
ESTRUC_ *threeargs (ESTRUC_ *,              /* fun(x, y, z)  code */
                        ESTRUC_ *, ESTRUC_ *);
ESTRUC_ *twoargs (E_TYPE_, ESTRUC_ *,       /* fun(x, y)  code */
                               ESTRUC_ *);
ESTRUC_ *unequal (ESTRUC_ *, ESTRUC_ *);   /* != code */
ESTRUC_ *while_stmnt (ESTRUC_ *, ESTRUC_ *);/* while code */
ESTRUC_ *young (ESTRUC_ *, ESTRUC_ *);     /* younger code */
ESTRUC_ *xor  (ESTRUC_ *, ESTRUC_ *);           /* ^ (binary) code */
ESTRUC_ *zeroargs (E_TYPE_);               /* fun()  code */

void    addpatch (unsigned *, unsigned,     /* add value to patch-list */
                               unsigned);
void    backend (void);                /* finish s_bin construction */
void    btoi (ESTRUC_ *);              /* boolean to int */
void    callrss (ESTRUC_ *, FUNNR_      /* call rss function */
                                MARG);     /* and add asp, xxx instruction */
void    callhidden(int, ESTRUC_ *);         /* call hidden function */
void    catargs (ESTRUC_ *);           /* arguments to code */
void    catstrings (ESTRUC_ *,          /* catenate string consts */
                             ESTRUC_ *);
void    change_file (char *);          /* switch to other file */
void    clearbin (ESTRUC_ *,            /* clear and init 2 ESTRUC_s */
                      ESTRUC_ *);
void    clear_hidden(void);                 /* clear hidden function-names */
void    close_fun (ESTRUC_ *);         /* close a function def. */
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
void    make_frame (void);             /* generate op_frame */
void    gencode (ESTRUC_ *, OPCODE_     /* append new code */
                                   MARG);
void    open_fun (void);               /* open a function */
void    outbin (void *, unsigned);     /* write INT8s to s_bin */
void    outcode (ESTRUC_ *, int,        /* append code to e->code */
                        unsigned);
void    patchfalse (ESTRUC_ *);        /* jmp_false target */
void    patchtrue (ESTRUC_ *);         /* jmp_true target */
void    patchup (INT8 *, unsigned,      /* patchup t/f list */
                     unsigned *, unsigned, int);
void    patchup_true (ESTRUC_ *, int); /* batchpatch truelist */
void    patchup_false (ESTRUC_ *, int);/* batchpatch truelist */
void    pop_dead(void);                     /* restore dead-level */
void    push_dead(void);                    /* new dead-level */
void    semantic (char * MARG);        /* give semantic error */
