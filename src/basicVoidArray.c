/*****************************************************************************/
/*                                                                           */
/*                       Random Polygon Generator (RPG)                      */
/*                                                                           */
/* Copyright (C)  T. Auer, M. Gschwandtner, M. Heimlich, M. Held   1994-2020 */
/*                                                                           */
/*****************************************************************************/

#include "basicVoidArray.h"
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

void VAinit(t_voidArray *anVoidArray, int nrOfVoids)
{
  void *curVoid;  
  int count;  

  if (nrOfVoids > 0)
  {
    anVoidArray->nrOfVoids = nrOfVoids;  
    anVoidArray->array = (void *)
     ercalloc(anVoidArray->nrOfVoids, sizeof(void *));  

    curVoid = anVoidArray->array;  

    for (count=1;  count<=anVoidArray->nrOfVoids;  count++)
    {
      curVoid = NULL;  
      curVoid++;  
    }
  }
  else
    anVoidArray->nrOfVoids = 0;  
}


void *VAgetVoid(t_voidArray *anVoidArray, int index)
{
  if ((index <= anVoidArray->nrOfVoids) && (1 <= index))
    return *(anVoidArray->array+index-1);  
}


void VAsetVoid(t_voidArray *anVoidArray, int index, void *newValue)
{
	if ((index <= anVoidArray->nrOfVoids) && (1 <= index))
  {
    *(anVoidArray->array+index-1) = newValue;  
//    anVoidArray->array[index-1] = newValue;  

	  }
}

int VAsizeOf(t_voidArray *anVoidArray)
{
  return(anVoidArray->nrOfVoids);  
}

void VAcopyArray(t_voidArray *sourceArray, t_voidArray *destArray)
{
  /*  bcopy(sourceArray->array, destArray->array, 
      sourceArray->nrOfDoubles*sizeof(double));  */
  memcpy(destArray->array, sourceArray->array, 
        sourceArray->nrOfVoids*sizeof(void *));  
}

void VAfree(t_voidArray *anVoidArray)
{
  if (anVoidArray->nrOfVoids > 0)
    erfree(anVoidArray->array);  

  anVoidArray->nrOfVoids = 0;  
}

void VArealloc(t_voidArray *anVoidArray, int newNrOfVoids)
{
  anVoidArray->array = 
    (void *)errealloc(anVoidArray->array, sizeof(void *)*newNrOfVoids);  
  anVoidArray->nrOfVoids = newNrOfVoids;  
}
