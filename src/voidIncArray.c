/*****************************************************************************/
/*                                                                           */
/*                       Random Polygon Generator (RPG)                      */
/*                                                                           */
/* Copyright (C)  T. Auer, M. Gschwandtner, M. Heimlich, M. Held   1994-2020 */
/*                                                                           */
/*****************************************************************************/

#include "basicDefs.h"
#include "voidIncArray.h"
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

void VIinitArray(t_vIncArray *anArray, int initialSize)
{
  anArray->growthSize = MAX(initialSize, 20);  
  anArray->curNrOfPoints = 0;  
  VAinit(&(anArray->array), initialSize);  
}


void *VIgetVoid(t_vIncArray *anArray, int index)
{
  void *getElem;  
  
  if (index <= anArray->curNrOfPoints)
    getElem = VAgetVoid(&(anArray->array), index);  
  else
    getElem = NULL;  
   
  return(getElem);  
}

void VIsetVoid(t_vIncArray *anArray, int index, void *newValue)
{
  if (index <= anArray->curNrOfPoints)
    VAsetVoid(&(anArray->array), index, newValue);  
}


void VIaddVoid(t_vIncArray *anArray, void *newVoid)
{
	//printf ("1 addVoid curNrOfPoints: %d\n", anArray->curNrOfPoints);  

	anArray->curNrOfPoints++;  
	//printf ("2 addVoid curNrOfPoints: %d\n", anArray->curNrOfPoints);  
  if (anArray->curNrOfPoints > VAsizeOf(&(anArray->array)))
    /* current allocated array is already filled, allocate new
       array and copy points */
    VArealloc(&(anArray->array), 
		   VAsizeOf(&(anArray->array))+anArray->growthSize);  

    VAsetVoid(&(anArray->array), anArray->curNrOfPoints, newVoid);  
}


int VIsizeOf(t_vIncArray *anArray)
{
  return(anArray->curNrOfPoints);  
}

void VIfreeArray(t_vIncArray *anArray)
{
  VAfree(&(anArray->array));  
  anArray->growthSize = 0;  
  anArray->curNrOfPoints = 0;  
}

void VIfreezeArray(t_vIncArray *anArray)
{
  if (VIsizeOf(anArray) > 0)
    VArealloc(&(anArray->array), anArray->curNrOfPoints);  
  else
    VIfreeArray(anArray);  

  anArray->growthSize = 20;   
}

t_voidArray *VIgetArray(t_vIncArray *anArray)
{
  return(&(anArray->array));  
}
