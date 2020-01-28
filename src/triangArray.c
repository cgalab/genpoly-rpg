/*****************************************************************************/
/*                                                                           */
/*                       Random Polygon Generator (RPG)                      */
/*                                                                           */
/* Copyright (C)  T. Auer, M. Gschwandtner, M. Heimlich, M. Held   1994-2020 */
/*                                                                           */
/*****************************************************************************/

#include "basicDefs.h"
#include "triangArray.h"
#include "eralloc.h"
#include <string.h>

/********************************************************************/
/*                                                                  */
/* Constants and the like                                           */
/*                                                                  */
/********************************************************************/



/********************************************************************/
/*                                                                  */
/* Procedures and functions                                         */
/*                                                                  */
/********************************************************************/

void TRinitEmptyArray(t_triangArray *anArray)
{
  TRinitArray(anArray, 0);  
  anArray->curNrOfTriangs = 0;  
  anArray->growthSize = 20;  
}


void TRinitArray(t_triangArray *anArray, int nrOfTriangs)
{
  anArray->nrOfTriangs = nrOfTriangs;  
  anArray->curNrOfTriangs = 0;  
  anArray->growthSize = MAX(20, nrOfTriangs);  

  if (anArray->nrOfTriangs > 0)
    {
      anArray->array = (t_triangle *)
	ercalloc(anArray->nrOfTriangs, sizeof(t_triangle));  
    }
  else 
    anArray->array = NULL;  
}


t_triangle TRgetTriang(t_triangArray *anArray, int index)
{
  t_triangle result;  
  
  result.vertex[0] = -1;  
  result.vertex[1] = -1;  
  result.vertex[2] = -1;  

  if ((index <= anArray->nrOfTriangs) && (1 <= index) &&
      (index <= anArray->curNrOfTriangs))
    result = *(anArray->array+index-1);  

  return(result);  
}


void TRsetTriang(t_triangArray *anArray, int index, t_triangle newTriang)
{
  if ((index <= anArray->nrOfTriangs) && (1 <= index) &&
      (index <= anArray->curNrOfTriangs))
    *(anArray->array+index-1) = newTriang;  
}


int TRnrOfTriangs(t_triangArray *anArray)
{
  return(anArray->curNrOfTriangs);  
}

void TRfreeArray(t_triangArray *anArray)
{
  if (anArray->nrOfTriangs > 0)
    erfree(anArray->array);  
  
  anArray->nrOfTriangs = 0;  
}


void TRreallocArray(t_triangArray *anArray, int newNrOfTriangs)
{
  anArray->array = 
    (t_triangle *)errealloc(anArray->array, sizeof(t_triangle)*newNrOfTriangs);  
  
  anArray->nrOfTriangs = newNrOfTriangs;  
}


void TRaddTriang(t_triangArray *anArray, t_triangle newTriang)
{
  anArray->curNrOfTriangs++;  

  if (anArray->curNrOfTriangs > anArray->nrOfTriangs)
    /* current allocated array is already filled, allocate new
       array and copy points */
    TRreallocArray(anArray, anArray->nrOfTriangs+anArray->growthSize);  

    TRsetTriang(anArray, anArray->curNrOfTriangs, newTriang);  
}


void TRfreezeArray(t_triangArray *anArray)
{
  if (TRnrOfTriangs(anArray) > 0)
    TRreallocArray(anArray, anArray->curNrOfTriangs);  
  else
    TRfreeArray(anArray);  

  anArray->growthSize = 20;   
}


void TRcopyArray(t_triangArray *sourceArray, t_triangArray *destArray)
{
  if (sourceArray->nrOfTriangs > 0)
    memcpy(destArray->array, sourceArray->array, 
	   sourceArray->nrOfTriangs*sizeof(t_triangle));  
  
  destArray->nrOfTriangs = sourceArray->nrOfTriangs;  
  destArray->curNrOfTriangs = sourceArray->curNrOfTriangs;  
  destArray->growthSize = sourceArray->growthSize;  
}

