/*****************************************************************************/
/*                                                                           */
/*                       Random Polygon Generator (RPG)                      */
/*                                                                           */
/* Copyright (C)  T. Auer, M. Gschwandtner, M. Heimlich, M. Held   1994-2020 */
/*                                                                           */
/*****************************************************************************/

#include "polyArray.h"
#include "eralloc.h"

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

void YAinitEmptyArray(t_polyArray *anArray)
{
  YAinitArray(anArray, 0);  
}

void YAinitArray(t_polyArray *anArray, int nrOfPolys)
{
  int count;  

  anArray->nrOfPolys = nrOfPolys;  

  if (anArray->nrOfPolys > 0)
    {
      anArray->array = (t_polygon *)
	ercalloc(anArray->nrOfPolys, sizeof(t_polygon));  
    }
  
  for (count=0;  count<nrOfPolys;  count++)
    {
      BPinit((anArray->array+count), 0);  
    }
  
}

t_polygon *YAgetPoly(t_polyArray *anArray, int index)
{
  t_polygon *getElem;  

  if ((index <= anArray->nrOfPolys) && (1 <= index))
    getElem = (anArray->array+index-1);  
  else
    getElem = NULL;  

  return(getElem);  
}

void YAsetPoly(t_polyArray *anArray, int index, t_polygon *newPoly)
{
  if ((index <= anArray->nrOfPolys) && (1 <= index))
    {
      BPfree((anArray->array+index-1));  
      BPinit((anArray->array+index-1), BPsizeOf(newPoly));  
      BPcopyPoly(newPoly, (anArray->array+index-1));  
    }
}


int YAnrOfPolys(t_polyArray *anArray)
{
  return(anArray->nrOfPolys);  
}

void YAfreeArray(t_polyArray *anArray)
{
  int count;  

  if (anArray->nrOfPolys > 0)
    {
      for (count=0;  count<anArray->nrOfPolys;  count++)
	{
	  BPfree((anArray->array+count));  
	}
    
      erfree(anArray->array);  
    }

  anArray->nrOfPolys = 0;  
}

void YAreallocArray(t_polyArray *anArray, int newNrOfPolys)
{
  int count;  
  int oldSize;  
  
  oldSize = anArray->nrOfPolys;  

  anArray->array = 
    (t_polygon *)errealloc(anArray->array, sizeof(t_polygon)*newNrOfPolys);  
  
  if (oldSize < newNrOfPolys)
    {
      for (count = oldSize;  count<newNrOfPolys;  count++)
	{
	  BPinit((anArray->array+count), 0);  
	}
    }

  anArray->nrOfPolys = newNrOfPolys;  
}

