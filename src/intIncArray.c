/*****************************************************************************/
/*                                                                           */
/*                       Random Polygon Generator (RPG)                      */
/*                                                                           */
/* Copyright (C)  T. Auer, M. Gschwandtner, M. Heimlich, M. Held   1994-2020 */
/*                                                                           */
/*****************************************************************************/

#include "basicDefs.h"
#include "intIncArray.h"
#include <stdlib.h>

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

void IIinitArray(t_iIncArray *anArray, int initialSize)
{
  anArray->growthSize = MAX(initialSize, 20);  
  anArray->curNrOfPoints = 0;  
  IAinit(&(anArray->array), initialSize);  
}


int IIgetInt(t_iIncArray *anArray, int index)
{
  int getElem;  
  
  if (index <= anArray->curNrOfPoints)
    getElem = IAgetInt(&(anArray->array), index);  
  else
    getElem = -1;  
   
  return(getElem);  
}

void IIsetInt(t_iIncArray *anArray, int index, int newValue)
{
  if (index <= anArray->curNrOfPoints)
    IAsetInt(&(anArray->array), index, newValue);  
}


void IIaddInt(t_iIncArray *anArray, int newInt)
{
  anArray->curNrOfPoints++;  

  if (anArray->curNrOfPoints > IAsizeOf(&(anArray->array)))
    /* current allocated array is already filled, allocate new
       array and copy points */
    IArealloc(&(anArray->array), 
		   IAsizeOf(&(anArray->array))+anArray->growthSize);  

    IAsetInt(&(anArray->array), anArray->curNrOfPoints, newInt);  
}


int IIsizeOf(t_iIncArray *anArray)
{
  return(anArray->curNrOfPoints);  
}

void IIfreeArray(t_iIncArray *anArray)
{
  IAfree(&(anArray->array));  
  anArray->growthSize = 0;  
  anArray->curNrOfPoints = 0;  
}

void IIfreezeArray(t_iIncArray *anArray)
{
  if (IIsizeOf(anArray) > 0)
    IArealloc(&(anArray->array), anArray->curNrOfPoints);  
  else
    IIfreeArray(anArray);  

  anArray->growthSize = 20;   
}

t_intArray *IIgetArray(t_iIncArray *anArray)
{
  return(&(anArray->array));  
}


int IIcompPoints(const void *elem1, const void *elem2)
{
  /* NOTE: this function returns -1 if elem1 < elem2 */
  int res;  
  int *int1, *int2;  
  int1 = (int *) elem1;  
  int2 = (int *) elem2;  

  if (*int1 == *int2)
    res = 0;  
  else if (*int1 < *int2)
    res = -1;  
  else
    res = 1;  

  return(res);  
}

void IIsortArray(t_iIncArray *anArray)
{
  qsort(anArray->array.array, IIsizeOf(anArray), sizeof(int), IIcompPoints);  
}
