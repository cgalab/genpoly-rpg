/*****************************************************************************/
/*                                                                           */
/*                       Random Polygon Generator (RPG)                      */
/*                                                                           */
/* Copyright (C)  T. Auer, M. Gschwandtner, M. Heimlich, M. Held   1994-2020 */
/*                                                                           */
/*****************************************************************************/

#include "basicIntArray.h"
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

void IAinit(t_intArray *anIntArray, int nrOfInts)
{
  int *curInt;  
  int count;  

  if (nrOfInts > 0)
  {
    anIntArray->nrOfInts = nrOfInts;  
    anIntArray->array = (int *)
     ercalloc(anIntArray->nrOfInts, sizeof(int));  

    curInt = anIntArray->array;  

    for (count=1;  count<=anIntArray->nrOfInts;  count++)
    {
      *curInt = -1;  
      curInt++;  
    }
  }
  else
    anIntArray->nrOfInts = 0;  
}


int IAgetInt(t_intArray *anIntArray, int index)
{
  int result = -1;  

  if ((index <= anIntArray->nrOfInts) && (1 <= index))
    result = *(anIntArray->array+index-1);  

  return(result);  
}


void IAsetInt(t_intArray *anIntArray, int index, int newValue)
{
  int *setInt;  
  if ((index <= anIntArray->nrOfInts) && (1 <= index))
  {
    setInt = (anIntArray->array+index-1);  
    *setInt = newValue;  
  }
}

void IAinc(t_intArray *anIntArray, int index)
{
  int *setInt;  
  if ((index <= anIntArray->nrOfInts) && (1 <= index))
  {
    setInt = (anIntArray->array+index-1);  
    (*setInt)++;  
  }
}

void IAdec(t_intArray *anIntArray, int index)
{
  int *setInt;  
  if ((index <= anIntArray->nrOfInts) && (1 <= index))
  {
    setInt = (anIntArray->array+index-1);  
    (*setInt)--;  
  }
}


int IAsizeOf(t_intArray *anIntArray)
{
  return(anIntArray->nrOfInts);  
}

void IAcopyArray(t_intArray *sourceArray, t_intArray *destArray)
{
  /*  bcopy(sourceArray->array, destArray->array, 
      sourceArray->nrOfInts*sizeof(int));  */
  memcpy(destArray->array, sourceArray->array, 
        sourceArray->nrOfInts*sizeof(int));  
}

void IAfree(t_intArray *anIntArray)
{
  if (anIntArray->nrOfInts > 0)
    erfree(anIntArray->array);  

  anIntArray->nrOfInts = 0;  
}

void IArealloc(t_intArray *anIntArray, int newNrOfInts)
{
  anIntArray->array = 
    (int *)errealloc(anIntArray->array, sizeof(int)*newNrOfInts);  
  anIntArray->nrOfInts = newNrOfInts;  
}





