/*****************************************************************************/
/*                                                                           */
/*                       Random Polygon Generator (RPG)                      */
/*                                                                           */
/* Copyright (C)  T. Auer, M. Gschwandtner, M. Heimlich, M. Held   1994-2020 */
/*                                                                           */
/*****************************************************************************/

#include "quadEdge.h"
#include "basicLine.h"
#include "lineInfo.h"
#include "pointArray.h"
#include "basicDefs.h"

#ifndef __SWEEPARRANGE_H_
#define __SWEEPARRANGE_H_

/********************************************************************/
/*                                                                  */
/* Constants and the like                                           */
/*                                                                  */
/********************************************************************/

/* event types for the sweepline */
#define NORMAL             1
#define VERTICAL           2
#define HULL_UPDATE        3

/********************************************************************/
/*                                                                  */
/* Data Types                                                       */
/*                                                                  */
/********************************************************************/

typedef struct {
  int eventType;  
  int pIndex;  
  t_point point;  
  t_lInfoP line1, line2;  
} t_event;  

typedef struct {
  t_edgeInfo *firstEdge;  
} t_arrange;  

/********************************************************************/
/*                                                                  */
/* Procedures and functions                                         */
/*                                                                  */
/********************************************************************/

void SAcreate(t_arrange *anArrange, t_pointArray *pArray);  

/* for each kernel defined by the arangement, calculate one point 
   within the kernel and store it within kernelPoints */
void SAkernels(t_pointArray *kernelPoints, t_arrange *anArrange, 
	       t_pointArray *pArray);  

void SAfree(t_arrange *anArrange);  

#endif
