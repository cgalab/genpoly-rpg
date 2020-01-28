/*****************************************************************************/
/*                                                                           */
/*                       Random Polygon Generator (RPG)                      */
/*                                                                           */
/* Copyright (C)  T. Auer, M. Gschwandtner, M. Heimlich, M. Held   1994-2020 */
/*                                                                           */
/*****************************************************************************/

#include "basicIntArray.h"
#include "basicPoly.h"
#ifndef __BASICPOLYLINE_H_
#define __BASICPOLYLINE_H_

/********************************************************************/
/*                                                                  */
/* Constants and the like                                           */
/*                                                                  */
/********************************************************************/


/********************************************************************/
/*                                                                  */
/* Data Types                                                       */
/*                                                                  */
/********************************************************************/

typedef struct{
  t_polygon polyArray;  
  int curNrOfPoints;  
} t_polyLine;  

/********************************************************************/
/*                                                                  */
/* Procedures and functions                                         */
/*                                                                  */
/********************************************************************/

void BLinit(t_polyLine *aPolyLine, int maxNrOfPoints);  

int BLgetPIndex(t_polyLine *aPolyLine, int index);  

void BLaddPIndex(t_polyLine *aPolyLine, int value);  

void BLdelLastIndex(t_polyLine *aPolyLine);  

int BLsizeOf(t_polyLine *aPolyLine);  

void BLcopyPoly(t_polyLine *sourcePolyLine, t_polyLine *destPolyLine);  

void BLfree(t_polyLine *aPolyLine);  

#endif
