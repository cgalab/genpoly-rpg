/*****************************************************************************/
/*                                                                           */
/*                       Random Polygon Generator (RPG)                      */
/*                                                                           */
/* Copyright (C)  T. Auer, M. Gschwandtner, M. Heimlich, M. Held   1994-2020 */
/*                                                                           */
/*****************************************************************************/

#include <stdio.h>
#include "pointArray.h"
#include "basicPoly.h"

#ifndef __IPEWRITE_H_
#define __IPEWRITE_H_

/********************************************************************/
/*                                                                  */
/* Constants and the like                                           */
/*                                                                  */
/********************************************************************/

#define IPE_MIN_X          5.0
#define IPE_MIN_Y          5.0
#define IPE_MAX_X        452.0
//#define IPE_MAX_Y        568.0
#define IPE_MAX_Y        452.0

/********************************************************************/
/*                                                                  */
/* Data Types                                                       */
/*                                                                  */
/********************************************************************/



/********************************************************************/
/*                                                                  */
/* Procedures and functions                                         */
/*                                                                  */
/********************************************************************/
FILE *IWopenFile(char *fileName);  

void IWwritePoints(FILE *ipeFile, t_pointArray *pArray);  

void IWwritePoly(FILE *ipeFile, t_polygon *aPoly, t_pointArray *pArray);  

void IWcloseFile(FILE *ipeFile);  

#endif
