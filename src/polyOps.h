/*****************************************************************************/
/*                                                                           */
/*                       Random Polygon Generator (RPG)                      */
/*                                                                           */
/* Copyright (C)  T. Auer, M. Gschwandtner, M. Heimlich, M. Held   1994-2020 */
/*                                                                           */
/*****************************************************************************/

#include "pointList.h"
#include "basicPoly.h"

#ifndef __POLYOPS_H_
#define __POLYOPS_H_

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


/********************************************************************/
/*                                                                  */
/* Procedures and functions                                         */
/*                                                                  */
/********************************************************************/
void YOinit();  

void YOfree();  

void YOclearPoly();  

void YOipeWrite(FILE *ipeFile);  

void YOsmoothPoly(t_pointList *pList);  

void YObouncePoly(int nrOfMoves, t_pointList *pList);  

void YOxmonoPoly(t_pointList *pList);  

void YOstarPoly(t_pointList *pList);  

void YOfastStarPoly(t_pointList *pList);  

void YOnewStarPoly(t_pointList *pList);  

void YOgrowingPoly(t_pointList *pList);  

void YOgrowingPolyII(t_pointList *pList);  

void YO2optPoly(t_pointList *pList);  

void YO2optPolyII(t_pointList *pList);  

void YO2optPolyIII(t_pointList *pList);  

void YOspPoly(t_pointList *pList);  

void YOrejectPoly(t_pointList *pList);  

void YOsearchPoly(t_pointList *pList);  

void YOtriangPoly(int nrOfPoints, t_pointList *pList);  

void YOholesPoly(int nrOfHoles, t_pointList *pList);  

void YOdrawPoly();  

void YOloadPoly(char *fileName, t_pointList *pList);  

void YOwritePoly(char *fileName);  

void YOwriteDat(char *fileName);  

void YOwriteLine(char *fileName);  

void YOlengthDist(char *fileName, int nrOfClasses);  

void YOslopeDist(char *fileName, int nrOfClasses);  

void YOangleDist(char *fileName, int nrOfClasses);  

void YOsinuosity(char *fileName);  

/* void YOlinkDiam(char *fileName);  */

void YOanalysisII();  

void YOsinuosityII();  

void YOgetPoly(t_polygon **polygon, t_pointArray **points);  

#endif
