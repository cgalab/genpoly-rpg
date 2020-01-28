/*****************************************************************************/
/*                                                                           */
/*                       Random Polygon Generator (RPG)                      */
/*                                                                           */
/* Copyright (C)  T. Auer, M. Gschwandtner, M. Heimlich, M. Held   1994-2020 */
/*                                                                           */
/*****************************************************************************/

#include "uLongArray.h"
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

void ULinit(t_uLongArray *anArray, int nrOfULongs)
{
  unsigned long *curULong;  
  int count;  

  if (nrOfULongs > 0)
  {
    anArray->nrOfULongs = nrOfULongs;  
    anArray->array = (unsigned long *)
     ercalloc(anArray->nrOfULongs, sizeof(unsigned long));  

    curULong = anArray->array;  

    for (count=1;  count<=anArray->nrOfULongs;  count++)
    {
      *curULong = 0;  
      curULong++;  
    }
  }
  else
    anArray->nrOfULongs = 0;  
}


unsigned long ULgetULong(t_uLongArray *anArray, int index)
{
  unsigned long result = 0;  

  if ((index <= anArray->nrOfULongs) && (1 <= index))
    result = *(anArray->array+index-1);  

  return(result);  
}


void ULsetULong(t_uLongArray *anArray, int index, unsigned long newValue)
{
  unsigned long *setLong;  

  if ((index <= anArray->nrOfULongs) && (1 <= index))
  {
    setLong = (anArray->array+index-1);  
    *setLong = newValue;  
  }
}

void ULinc(t_uLongArray *anArray, int index)
{
  unsigned long *setLong;  

  if ((index <= anArray->nrOfULongs) && (1 <= index))
  {
    setLong = (anArray->array+index-1);  
    (*setLong)++;  
  }
}

void ULdec(t_uLongArray *anArray, int index)
{
  unsigned long *setLong;  

  if ((index <= anArray->nrOfULongs) && (1 <= index))
  {
    setLong = (anArray->array+index-1);  
    (*setLong)--;  
  }
}


int ULsizeOf(t_uLongArray *anArray)
{
  return(anArray->nrOfULongs);  
}

void ULcopyArray(t_uLongArray *sourceArray, t_uLongArray *destArray)
{
  memcpy(destArray->array, sourceArray->array, 
        sourceArray->nrOfULongs*sizeof(unsigned long));  
}

void ULfree(t_uLongArray *anArray)
{
  if (anArray->nrOfULongs > 0)
    erfree(anArray->array);  

  anArray->nrOfULongs = 0;  
}

void ULrealloc(t_uLongArray *anArray, int newNrOfULongs)
{
  anArray->array = (unsigned long *)
    errealloc(anArray->array, sizeof(unsigned long)*newNrOfULongs);  
  anArray->nrOfULongs = newNrOfULongs;  
}





