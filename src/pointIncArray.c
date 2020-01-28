/*****************************************************************************/
/*                                                                           */
/*                       Random Polygon Generator (RPG)                      */
/*                                                                           */
/* Copyright (C)  T. Auer, M. Gschwandtner, M. Heimlich, M. Held   1994-2020 */
/*                                                                           */
/*****************************************************************************/

#include "pointIncArray.h"
#include "basicGeom.h"

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

void PIinitArray(t_pIncArray *anArray, int initialSize)
{
  anArray->growthSize = MAX(initialSize, 20);  
  anArray->curNrOfPoints = 0;  
  PAinitArray(&(anArray->array), initialSize);  
}


t_point PIgetPoint(t_pIncArray *anArray, int index)
{
  t_point getElem;  
  
  if (index <= anArray->curNrOfPoints)
    getElem = PAgetPoint(&(anArray->array), index);  
  else
    getElem = BGdefaultPoint;  

  return(getElem);  
}


void PIaddPoint(t_pIncArray *anArray, t_point newPoint)
{
  anArray->curNrOfPoints++;  

  if (anArray->curNrOfPoints > PAnrOfPoints(&(anArray->array)))
    /* current allocated array is already filled, allocate new
       array and copy points */
    PAreallocArray(&(anArray->array), 
		   PAnrOfPoints(&(anArray->array))+anArray->growthSize);  

    PAsetPoint(&(anArray->array), anArray->curNrOfPoints, newPoint);  
}


int PInrOfPoints(t_pIncArray *anArray)
{
  return(anArray->curNrOfPoints);  
}

void PIfreeArray(t_pIncArray *anArray)
{
  PAfreeArray(&(anArray->array));  
  anArray->growthSize = 0;  
  anArray->curNrOfPoints = 0;  
}

void PIfreezeArray(t_pIncArray *anArray)
{
  if (PInrOfPoints(anArray) > 0)
    PAreallocArray(&(anArray->array), anArray->curNrOfPoints);  
  else
    PIfreeArray(anArray);  

  anArray->growthSize = 20;   
}

t_pointArray *PIgetArray(t_pIncArray *anArray)
{
  return(&(anArray->array));  
}
