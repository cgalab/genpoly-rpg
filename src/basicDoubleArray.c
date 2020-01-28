/*****************************************************************************/
/*                                                                           */
/*                       Random Polygon Generator (RPG)                      */
/*                                                                           */
/* Copyright (C)  T. Auer, M. Gschwandtner, M. Heimlich, M. Held   1994-2020 */
/*                                                                           */
/*****************************************************************************/

#include "basicDoubleArray.h"
#include "basicDefs.h"
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

void DAinit(t_doubleArray *anDoubleArray, int nrOfDoubles)
{
  double *curDouble;  
  int count;  

  if (nrOfDoubles > 0)
  {
    anDoubleArray->nrOfDoubles = nrOfDoubles;  
    anDoubleArray->array = (double *)
     ercalloc(anDoubleArray->nrOfDoubles, sizeof(double));  

    curDouble = anDoubleArray->array;  

    for (count=1;  count<=anDoubleArray->nrOfDoubles;  count++)
    {
      *curDouble = -1.0;  
      curDouble++;  
    }
  }
  else
    anDoubleArray->nrOfDoubles = 0;  
}


double DAgetDouble(t_doubleArray *anDoubleArray, int index)
{
  double result = -1;  

  if ((index <= anDoubleArray->nrOfDoubles) && (1 <= index))
    result = *(anDoubleArray->array+index-1);  

  return(result);  
}


void DAsetDouble(t_doubleArray *anDoubleArray, int index, double newValue)
{
  double *setDouble;  
  if ((index <= anDoubleArray->nrOfDoubles) && (1 <= index))
  {
    setDouble = (anDoubleArray->array+index-1);  
    *setDouble = newValue;  
  }
}

int DAsizeOf(t_doubleArray *anDoubleArray)
{
  return(anDoubleArray->nrOfDoubles);  
}

void DAcopyArray(t_doubleArray *sourceArray, t_doubleArray *destArray)
{
  /*  bcopy(sourceArray->array, destArray->array, 
      sourceArray->nrOfDoubles*sizeof(double));  */
  memcpy(destArray->array, sourceArray->array, 
        sourceArray->nrOfDoubles*sizeof(double));  
}

void DAfree(t_doubleArray *anDoubleArray)
{
  if (anDoubleArray->nrOfDoubles > 0)
    erfree(anDoubleArray->array);  

  anDoubleArray->nrOfDoubles = 0;  
}

void DArealloc(t_doubleArray *anDoubleArray, int newNrOfDoubles)
{
  anDoubleArray->array = 
    (double *)errealloc(anDoubleArray->array, sizeof(double)*newNrOfDoubles);  
  anDoubleArray->nrOfDoubles = newNrOfDoubles;  
}





