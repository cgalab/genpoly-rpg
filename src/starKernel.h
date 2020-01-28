/*****************************************************************************/
/*                                                                           */
/*                       Random Polygon Generator (RPG)                      */
/*                                                                           */
/* Copyright (C)  T. Auer, M. Gschwandtner, M. Heimlich, M. Held   1994-2020 */
/*                                                                           */
/*****************************************************************************/

#include "pointArray.h"
#include "basicLine.h"
#include "polyIncArray.h"

#ifndef __STARKERNEL_H_
#define __STARKERNEL_H_

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
typedef struct {
  t_point point;  
  int pIndex1, pIndex2;  
} t_isectPoint;  


/********************************************************************/
/*                                                                  */
/* Procedures and functions                                         */
/*                                                                  */
/********************************************************************/
t_line *SKinitLines(t_pointArray *pArray);  

void SKcreateLines(t_pointArray *pArray, t_line *minSortArray, 
		   t_line *maxSortArray);  

void SKcreatePolys(t_line *minLines, t_line *maxLines, 
		     t_yIncArray *kernelPoints, t_pointArray *pArray);  

void SKwritePolys(FILE *outFile, t_line *minLines, t_line *maxLines, 
		    t_pointArray *pArray);  

void SKfreeLines(t_line *lineArray);  

#endif
