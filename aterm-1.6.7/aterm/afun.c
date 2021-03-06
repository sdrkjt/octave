/*{{{  includes */

#include <ctype.h>
#include <stdlib.h>
#include <assert.h>
#include "aterm2.h"
#include "_afun.h"
#include "memory.h"
#include "util.h"
#include "debug.h"

#ifdef DMALLOC
#include <dmalloc.h>
#endif

/*}}}  */
/*{{{  defines */

#define INITIAL_AFUN_TABLE_CLASS 14

#define SYMBOL_HASH_SIZE	65353	/* nextprime(65335) */
#define SYMBOL_HASH_OPT		"-at-symboltable"
#define AFUN_TABLE_OPT		"-at-afuntable"

#define SHIFT_INDEX 1
#define SYM_GET_NEXT_FREE(sym)    ((MachineWord)(sym) >> SHIFT_INDEX)
#define SYM_SET_NEXT_FREE(next)   (1 | ((next) << SHIFT_INDEX))

#define INITIAL_PROTECTED_SYMBOLS   1024
#define SYM_PROTECT_EXPAND_SIZE     1024

#define MAGIC_PRIME 7

/*}}}  */
/*{{{  globals */

char afun_id[] = "$Id: afun.c,v 1.19 2002/01/16 13:19:38 jong Exp $";

static unsigned int table_class = INITIAL_AFUN_TABLE_CLASS;
static unsigned int table_size  = AT_TABLE_SIZE(INITIAL_AFUN_TABLE_CLASS);
static unsigned int table_mask  = AT_TABLE_MASK(INITIAL_AFUN_TABLE_CLASS);

static SymEntry *hash_table     = NULL;

static Symbol first_free = -1;

static Symbol *protected_symbols = NULL;
static int nr_protected_symbols  = 0;
static int max_protected_symbols  = 0;

/* Efficiency hack: was static */
SymEntry *at_lookup_table = NULL;
ATerm    *at_lookup_table_alias = NULL;

/*}}}  */
/*{{{  function declarations */

/* extern char *strdup(const char *s); */

/*}}}  */

/*{{{  static void resize_table() */

static void resize_table()
{
  int i;
  int new_class = table_class+1;
  int new_size  = AT_TABLE_SIZE(new_class);
  int new_mask  = AT_TABLE_MASK(new_class);

  at_lookup_table = (SymEntry *)realloc(at_lookup_table, new_size*sizeof(SymEntry));
  at_lookup_table_alias = (ATerm *)at_lookup_table;
  if (!at_lookup_table) {
    ATerror("afun.c:resize_table - could not allocate space for lookup table of %d afuns\n", new_size);
  }
  for (i = table_size; i < new_size; i++) {
    at_lookup_table[i] = (SymEntry) SYM_SET_NEXT_FREE(first_free);
    first_free = i;
  }

  hash_table = (SymEntry *)realloc(hash_table, new_size*sizeof(SymEntry));
  if (!hash_table) {
    ATerror("afun.c:resize_table - could not allocate space for hashtable of %d afuns\n", new_size);
  }
  memset(hash_table, 0, new_size*sizeof(SymEntry));

  for (i=0; i<table_size; i++) {
    SymEntry entry = at_lookup_table[i];
    if (!SYM_IS_FREE(entry)) {
      ShortHashNumber hnr = AT_hashSymbol(entry->name, GET_LENGTH(entry->header));
      hnr &= new_mask;
      entry->next = hash_table[hnr];
      hash_table[hnr] = entry;
    }
  }

  table_class = new_class;
  table_size  = new_size;
  table_mask  = new_mask;
}

/*}}}  */

/*{{{  unsigned int AT_symbolTableSize() */

unsigned int AT_symbolTableSize()
{
  return table_size;
}

/*}}}  */

/*{{{  void AT_initSymbol(int argc, char *argv[]) */
void AT_initSymbol(int argc, char *argv[])
{
  int i;
  AFun sym;

  for (i = 1; i < argc; i++) {
    if (streq(argv[i], SYMBOL_HASH_OPT)) {
      ATerror("Option %s is depcrecated, use %s instead!\n"
	      "Note that %s uses 2^<arg> as the actual table size.\n",
	      SYMBOL_HASH_OPT, AFUN_TABLE_OPT, AFUN_TABLE_OPT);
    } else if (streq(argv[i], AFUN_TABLE_OPT)) {
      table_class = atoi(argv[++i]);
      table_size  = AT_TABLE_SIZE(table_class);
      table_mask  = AT_TABLE_MASK(table_class);
    } else if(streq(argv[i], "-at-help")) {
      fprintf(stderr, "    %-20s: initial afun table class " 
	      "(default=%d)\n",	AFUN_TABLE_OPT " <class>", table_class);
    }
  }

  hash_table = (SymEntry *) calloc(table_size, sizeof(SymEntry));
  if (hash_table == NULL) {
    ATerror("AT_initSymbol: cannot allocate %d hash-entries.\n",
	    table_size);
  }

  at_lookup_table = (SymEntry *) calloc(table_size, sizeof(SymEntry));
  at_lookup_table_alias = (ATerm *)at_lookup_table;
  if (at_lookup_table == NULL) {
    ATerror("AT_initSymbol: cannot allocate %d lookup-entries.\n",
	    table_size);
  }

  for (i = first_free = 0; i < table_size; i++) {
    at_lookup_table[i] = (SymEntry) SYM_SET_NEXT_FREE(i+1);
  }

  at_lookup_table[i-1] = (SymEntry) SYM_SET_NEXT_FREE(-1);		/* Sentinel */

  protected_symbols = (Symbol *)calloc(INITIAL_PROTECTED_SYMBOLS, 
				       sizeof(Symbol));
  if(!protected_symbols) {
    ATerror("AT_initSymbol: cannot allocate initial protection buffer.\n");
  }
	
  sym = ATmakeAFun("<int>", 0, ATfalse);
  assert(sym == AS_INT);
  ATprotectAFun(sym);

  sym = ATmakeAFun("<real>", 0, ATfalse);
  assert(sym == AS_REAL);
  ATprotectAFun(sym);

  sym = ATmakeAFun("<blob>", 0, ATfalse);
  assert(sym == AS_BLOB);
  ATprotectAFun(sym);

  sym = ATmakeAFun("<_>", 1, ATfalse);
  assert(sym == AS_PLACEHOLDER);
  ATprotectAFun(sym);

  sym = ATmakeAFun("[_,_]", 2, ATfalse);
  assert(sym == AS_LIST);
  ATprotectAFun(sym);

  sym = ATmakeAFun("[]", 0, ATfalse);
  assert(sym == AS_EMPTY_LIST);
  ATprotectAFun(sym);

  sym = ATmakeAFun("{_}", 2, ATfalse);
  assert(sym == AS_ANNOTATION);
  ATprotectAFun(sym);
}
/*}}}  */

/*{{{  int AT_printSymbol(Symbol sym, FILE *f) */

/**
  * Print an afun.
  */

int AT_printSymbol(AFun fun, FILE *f)
{
  SymEntry entry = at_lookup_table[fun];
  char *id = entry->name;
  int size = 0;

  if (IS_QUOTED(entry->header)) {
    /* This function symbol needs quotes */
    fputc('"', f);
    size++;
    while(*id) {
      /* We need to escape special characters */
      switch(*id) {
      case '\\':
      case '"':
	fputc('\\', f);
	fputc(*id, f);
	size += 2;
	break;
      case '\n':
	fputc('\\', f);
	fputc('n', f);
	size += 2;
	break;
      case '\t':
	fputc('\\', f);
	fputc('t', f);
	size += 2;
	break;
      case '\r':
	fputc('\\', f);
	fputc('r', f);
	size += 2;
	break;
      default:
	fputc(*id, f);
	size++;
      }
      id++;
    }
    fputc('"', f);
    size++;
  } else {
    fputs(id, f);
    size += strlen(id);
  }
  return size;
}

/*}}}  */
/*{{{  int AT_writeAFun(AFun fun, byte_writer *writer) */

/**
  * Print an afun.
  */

int AT_writeAFun(AFun fun, byte_writer *writer)
{
  SymEntry entry = at_lookup_table[fun];
  char *id = entry->name;
  int size = 0;

  if (IS_QUOTED(entry->header)) {
    /* This function symbol needs quotes */
    write_byte('"', writer);
    size++;
    while(*id) {
      /* We need to escape special characters */
      switch(*id) {
      case '\\':
      case '"':
	write_byte('\\', writer);
	write_byte(*id, writer);
	size += 2;
	break;
      case '\n':
	write_byte('\\', writer);
	write_byte('n', writer);
	size += 2;
	break;
      case '\t':
	write_byte('\\', writer);
	write_byte('t', writer);
	size += 2;
	break;
      case '\r':
	write_byte('\\', writer);
	write_byte('r', writer);
	size += 2;
	break;
      default:
	write_byte(*id, writer);
	size++;
      }
      id++;
    }
    write_byte('"', writer);
    size++;
  } else {
    size += write_bytes(id, strlen(id), writer);
  }
  return size;
}

/*}}}  */

/*{{{  ShortHashNumber AT_hashSymbol(char *name, int arity) */

/**
 * Calculate the hash value of a symbol.
 */

ShortHashNumber AT_hashSymbol(char *name, int arity)
{
  ShortHashNumber hnr;
  char *walk = name;

  for(hnr = arity*3; *walk; walk++)
    hnr = 251 * hnr + *walk;
  
  return hnr*MAGIC_PRIME;
}


/*}}}  */

/*{{{  Symbol ATmakeSymbol(char *name, int arity, ATbool quoted) */

Symbol ATmakeSymbol(char *name, int arity, ATbool quoted)
{
  header_type header = SYMBOL_HEADER(arity, quoted);
  ShortHashNumber hnr = AT_hashSymbol(name, arity) & table_mask;
  SymEntry cur;
  
  if(arity >= MAX_ARITY) {
    ATabort("cannot handle symbols with arity %d (max=%d)\n",
	    arity, MAX_ARITY);
  }

  /* Find symbol in table */
  cur = hash_table[hnr];
  while (cur && (cur->header != header || !streq(cur->name, name))) {
    cur = cur->next;
  }
  
  if (cur == NULL) {
    Symbol free_entry;

    free_entry = first_free;
    if(free_entry == -1) {
      resize_table();

      /* Hashtable size changed, so recalculate hashnumber */
      hnr = AT_hashSymbol(name, arity) & table_mask;
     
      free_entry = first_free;
      if(free_entry == -1) {
	ATerror("AT_initSymbol: out of symbol slots!\n");
      }
    }

    first_free = SYM_GET_NEXT_FREE(at_lookup_table[first_free]);

    cur = (SymEntry) AT_allocate(TERM_SIZE_SYMBOL);
    at_lookup_table[free_entry] = cur;

    cur->header = header;
    cur->id = free_entry;
    cur->count = 0;
    cur->index = -1;

    cur->name = strdup(name);
    if (cur->name == NULL) {
      ATerror("ATmakeSymbol: no room for name of length %d\n", strlen(name));
    }

    cur->next = hash_table[hnr];
    hash_table[hnr] = cur;
  }

  return cur->id;
}

/*}}}  */
/*{{{  void AT_freeSymbol(SymEntry sym) */

/**
 * Free a symbol
 */

void AT_freeSymbol(SymEntry sym)
{
  ShortHashNumber hnr;
  ATerm t = (ATerm)sym;
  /*char *walk = sym->name;*/
  
  /* Assert valid name-pointer */
  assert(sym->name);
  
  /* Calculate hashnumber */
  hnr = AT_hashSymbol(sym->name, GET_LENGTH(sym->header));
  /*for(hnr = GET_LENGTH(sym->header)*3; *walk; walk++)
    hnr = 251 * hnr + *walk;*/
  hnr &= table_mask;
  
  /* Update hashtable */
  if (hash_table[hnr] == sym) {
    hash_table[hnr] = sym->next;
  } else {
    SymEntry cur, prev;
    prev = hash_table[hnr]; 
    for (cur = prev->next; cur != sym; prev = cur, cur = cur->next) {
      assert(cur != NULL);
    }
    prev->next = cur->next;
  }
  
  /* Free symbol name */
  free(sym->name);
  sym->name = NULL;
  
  at_lookup_table[sym->id] = (SymEntry)SYM_SET_NEXT_FREE(first_free);
  first_free = sym->id;
  
  /* Put the node in the appropriate free list */
  t->header = FREE_HEADER;
  t->next  = at_freelist[TERM_SIZE_SYMBOL];
  at_freelist[TERM_SIZE_SYMBOL] = t;
}

/*}}}  */
/*{{{  ATbool AT_findSymbol(char *name, int arity, ATbool quoted) */

/**
 * Check for the existence of a symbol
 */

ATbool AT_findSymbol(char *name, int arity, ATbool quoted)
{
  header_type header = SYMBOL_HEADER(arity, quoted);
  ShortHashNumber hnr = AT_hashSymbol(name, arity) & table_mask;
  SymEntry cur;
  
  if(arity >= MAX_ARITY)
    ATabort("cannot handle symbols with arity %d (max=%d)\n",
	    arity, MAX_ARITY);

  /* Find symbol in table */
  cur = hash_table[hnr];
  while (cur && (cur->header != header || !streq(cur->name, name)))
    cur = cur->next;
  
  return (cur == NULL) ? ATfalse : ATtrue;
}

/*}}}  */

/*{{{  ATbool AT_isValidSymbol(Symbol sym) */

/**
  * Check if a symbol is valid.
  */

ATbool AT_isValidSymbol(Symbol sym)
{
  return (sym >= 0 && sym < table_size
	  && !SYM_IS_FREE(at_lookup_table[sym])) ?  ATtrue : ATfalse;
}

/*}}}  */
/*{{{  void AT_unmarkSymbol(Symbol s) */

/**
  * Unmark a symbol by clearing its mark bit.
  */

/* <PO> This is now a macro
void AT_unmarkSymbol(Symbol s)
{
  at_lookup_table[s]->header &= ~MASK_MARK;
}
*/

/*}}}  */
/*{{{  ATbool AT_isMarkedSymbol(Symbol s) */

/**
  * Check the mark bit of a symbol.
  */

ATbool AT_isMarkedSymbol(Symbol s)
{
  return IS_MARKED(at_lookup_table[s]->header);
}

/*}}}  */

/*{{{  void ATprotectSymbol(Symbol sym) */

/**
	* Protect a symbol.
	*/

void ATprotectSymbol(Symbol sym)
{

  if(nr_protected_symbols >= max_protected_symbols) {
    max_protected_symbols += SYM_PROTECT_EXPAND_SIZE;
    protected_symbols = (Symbol *)realloc(protected_symbols,
					  max_protected_symbols * sizeof(Symbol));
    if(!protected_symbols)
      ATerror("ATprotectSymbol: no space to hold %d protected symbols.\n",
	      max_protected_symbols);
  }

  protected_symbols[nr_protected_symbols++] = sym;
}

/*}}}  */
/*{{{  void ATunprotectSymbol(Symbol sym) */

/**
	* Unprotect a symbol.
	*/

void ATunprotectSymbol(Symbol sym)
{
  int lcv;

  for(lcv = 0; lcv < nr_protected_symbols; ++lcv) {
    if(protected_symbols[lcv] == sym) {
      protected_symbols[lcv] = protected_symbols[--nr_protected_symbols];
      protected_symbols[nr_protected_symbols] = -1;
      break;
    }
  }
}

/*}}}  */
/*{{{  void AT_markProtectedSymbols() */

/**
 * Mark all symbols that were protected previously using ATprotectSymbol.
 */

void AT_markProtectedSymbols()
{
  int lcv;

  for(lcv = 0; lcv < nr_protected_symbols; lcv++) {
    SET_MARK(((ATerm)at_lookup_table[protected_symbols[lcv]])->header);
  }
}

/*}}}  */

