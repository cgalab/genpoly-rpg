/*****************************************************************************/
/*                                                                           */
/*                       Random Polygon Generator (RPG)                      */
/*                                                                           */
/* Copyright (C)  T. Auer, M. Gschwandtner, M. Heimlich, M. Held   1994-2020 */
/*                                                                           */
/*****************************************************************************/

#include <malloc.h>
#include <stdlib.h>
#include <string.h>

#ifndef __ERALLOC_H_
#define __ERALLOC_H_

/********************************************************************/
/*                                                                  */
/* Constants and the like                                           */
/*                                                                  */
/********************************************************************/



/********************************************************************/
/*                                                                  */
/* Data Types                                                       */
/*                                                                  */
/********************************************************************/



/********************************************************************/
/*                                                                  */
/* Procedures and functions                                         */
/*                                                                  */
/********************************************************************/

void erinit();  

void erend();  

void *eralloc(size_t size);  

void *ercalloc(size_t nrOfElems, size_t size);  

void *errealloc(void *pointer, size_t size);  

void erfree(void *pointer);  

#endif

