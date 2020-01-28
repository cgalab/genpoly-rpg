/*****************************************************************************/
/*                                                                           */
/*                       Random Polygon Generator (RPG)                      */
/*                                                                           */
/* Copyright (C)  T. Auer, M. Gschwandtner, M. Heimlich, M. Held   1994-2020 */
/*                                                                           */
/*****************************************************************************/

#include "eralloc.h"
#include "error.h"
#include <stdio.h>

/* if you dont want the memory allocation counter to be printed, 
   define the following constant */
#define MEM_COUNT /**/
#undef MEM_COUNT /**/

/* for debugging purposes, enable the following define */
#define MEM_DEBUG /**/
#undef MEM_DEBUG /**/

#ifdef MEM_DEBUG
#define MEM_COUNT
#endif

#ifdef NOX11_VERSION
#undef MEM_COUNT
#undef MEM_DEBUG
#endif 

static long ercount;  

#ifdef MEM_DEBUG
#include "basicIo.h"
static FILE *erfile;  
#endif
/********************************************************************/
/*                                                                  */
/* Procedures and functions                                         */
/*                                                                  */
/********************************************************************/

void erinit()
{
  ercount = 0;  
#ifdef MEM_DEBUG
  erfile  = FopenWrite("trace.mem");  
#endif
}

void erend()
{
#ifdef MEM_COUNT
  printf("MEMORY ALLOCATION COUNTER: %ld \n ", ercount);  
#endif
#ifdef MEM_DEBUG
  Fclose(erfile);  
#endif
}


void *eralloc(size_t size)
{
  void *allocMem;  

  if ((allocMem = (void *) malloc(size)) == NULL)
  {
#ifdef MEM_DEBUG
    fprintf(stderr, "ALLOC:    SIZE %10d\n", (int)size);  
#endif
    ERerror(ERR_MEM);  
  }

  ercount++;  

#ifdef MEM_DEBUG
  fprintf (erfile, "ALLOC:   %ld  SIZE: %10d\n", (long)allocMem, (int)size);  
#endif
  
  return(allocMem);  
}

void *ercalloc(size_t nrOfElems, size_t size)
{
  void *allocMem;  

  if ((allocMem = (void *) calloc(nrOfElems, size)) == NULL)
  {
#ifdef MEM_DEBUG
    fprintf(stderr, "CALLOC:   SIZE %10d  ELEMENTS: %10d\n", (int)size, 
	    (int)nrOfElems);  
#endif
    ERerror(ERR_MEM);  
  }

  ercount++;  
#ifdef MEM_DEBUG
  fprintf(erfile, "CALLOC:  %ld  SIZE: %10d  ELEMENTS: %10d\n", 
	  (long)allocMem, size, nrOfElems);  
#endif
  
   return(allocMem);  
}

void *errealloc(void *pointer, size_t size)
{
  void *allocMem;  

  if ((allocMem = (void *) realloc(pointer, size)) == NULL)
  {
#ifdef MEM_DEBUG
    fprintf(stderr, "REALLOC:  SIZE %10d\n", (int)size);  
#endif
    ERerror(ERR_MEM);  
  }

  if (pointer == NULL)
    ercount ++;  

#ifdef MEM_DEBUG 
  fprintf(erfile, "REALLOC: %ld  NEWADDRESS: %ld\n", (long)pointer, 
	  (long)allocMem);  
  fprintf(erfile, "REALLOC: %ld  SIZE: %10d\n", (long)allocMem, size);  
#endif

  return(allocMem);  
}


void erfree(void *pointer)
{
#ifdef MEM_DEBUG
  fprintf(erfile, "FREE:    %ld \n", (long)pointer);  
#endif

  ercount --;  

  free(pointer);  
}
  


