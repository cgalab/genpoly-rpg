/*****************************************************************************/
/*                                                                           */
/*                       Random Polygon Generator (RPG)                      */
/*                                                                           */
/* Copyright (C)  T. Auer, M. Gschwandtner, M. Heimlich, M. Held   1994-2020 */
/*                                                                           */
/*****************************************************************************/

#include "polyIncArray.h"

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

void YIinitArray(t_yIncArray *anArray, int initialSize)
{
  anArray->growthSize = MAX(initialSize, 20);  
  anArray->curNrOfPolys = 0;  
  YAinitArray(&(anArray->array), initialSize);  
}


t_polygon *YIgetPoly(t_yIncArray *anArray, int index)
{
  t_polygon *getElem;  
  
  if (index <= anArray->curNrOfPolys)
    getElem = YAgetPoly(&(anArray->array), index);  
  else
    getElem = NULL;  

  return(getElem);  
}


void YIaddPoly(t_yIncArray *anArray, t_polygon *newPoly)
{
  anArray->curNrOfPolys++;  

  if (anArray->curNrOfPolys > YAnrOfPolys(&(anArray->array)))
    /* current allocated array is already filled, allocate new
       array and copy Polys */
    YAreallocArray(&(anArray->array), 
		   YAnrOfPolys(&(anArray->array))+anArray->growthSize);  
  
  YAsetPoly(&(anArray->array), anArray->curNrOfPolys, newPoly);  
}


int YInrOfPolys(t_yIncArray *anArray)
{
  return(anArray->curNrOfPolys);  
}

void YIfreeArray(t_yIncArray *anArray)
{
  YAfreeArray(&(anArray->array));  
  anArray->growthSize = 0;  
  anArray->curNrOfPolys = 0;  
}

void YIfreezeArray(t_yIncArray *anArray)
{
  if (YInrOfPolys(anArray) > 0)
    YAreallocArray(&(anArray->array), anArray->curNrOfPolys);  
  else
    YIfreeArray(anArray);  

  anArray->growthSize = 20;   
}
