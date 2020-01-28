/*****************************************************************************/
/*                                                                           */
/*                       Random Polygon Generator (RPG)                      */
/*                                                                           */
/* Copyright (C)  T. Auer, M. Gschwandtner, M. Heimlich, M. Held   1994-2020 */
/*                                                                           */
/*****************************************************************************/

#include "basicPolyLine.h"
#include "basicDefs.h"

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

void BLinit(t_polyLine *aPolyLine, int maxNrOfPoints)
{
  BPinit((t_polygon*)&(aPolyLine->polyArray), maxNrOfPoints);  
  aPolyLine->curNrOfPoints = 0;  
}


int BLgetPIndex(t_polyLine *aPolyLine, int index)
{
  int res = -1;  

  if (index <= aPolyLine->curNrOfPoints)
    res = BPgetPIndex((t_polygon*)&(aPolyLine->polyArray), index);  

  return(res);  
}


void BLaddPIndex(t_polyLine *aPolyLine, int value)
{
  if (aPolyLine->curNrOfPoints <
      BPsizeOf((t_polygon*)&(aPolyLine->polyArray)))
  {
    aPolyLine->curNrOfPoints++;  
    BPsetPIndex((t_polygon*)&(aPolyLine->polyArray), aPolyLine->curNrOfPoints, 
                value);  
  }
}

void BLdelLastIndex(t_polyLine *aPolyLine)
{
  if (aPolyLine->curNrOfPoints > 0)
    aPolyLine->curNrOfPoints--;  
}


int BLsizeOf(t_polyLine *aPolyLine)
{
  return(aPolyLine->curNrOfPoints);  
}

void BLcopyPoly(t_polyLine *sourcePolyLine, t_polyLine *destPolyLine)
{
  BPcopyPoly((t_polygon*)&(sourcePolyLine->polyArray), 
             (t_polygon*)&(destPolyLine->polyArray));  
  destPolyLine->curNrOfPoints = sourcePolyLine->curNrOfPoints;  
}

void BLfree(t_polyLine *aPolyLine)
{
  BPfree((t_polygon*)&(aPolyLine->polyArray));  
  aPolyLine->curNrOfPoints = 0;  
}

