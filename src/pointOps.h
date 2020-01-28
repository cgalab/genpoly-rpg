/*****************************************************************************/
/*                                                                           */
/*                       Random Polygon Generator (RPG)                      */
/*                                                                           */
/* Copyright (C)  T. Auer, M. Gschwandtner, M. Heimlich, M. Held   1994-2020 */
/*                                                                           */
/*****************************************************************************/

#include "pointList.h"

#ifndef __POINTOPS_H_
#define __POINTOPS_H_

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

enum t_POactionState {NONE, ADD, DELETE};  

/********************************************************************/
/*                                                                  */
/* Procedures and functions                                         */
/*                                                                  */
/********************************************************************/

void POinit();  

void POfree();  

t_pointList* POgetPointList();  

void POloadPoints(char *fileName);  

void POwritePoints(char *fileName);  

void POdelPoint(double x, double y, double epsilon);  

void POaddPoint(double x, double y);  

void POclearPoints();  

void POdoAction(double x, double y, double epsilon);  

void POclearAction();  

void POsetActionAdd();  

void POsetActionDelete();  

void POipeWrite(FILE *ipeFile);  

void POdrawPoints();  

void POgenerateRandomPoints(int nrOfPoints);  

void POgenerateRandomCirclePoints(int nrOfPoints);  

void POgenerateClusterPoints(int nrOfPoints);  

#endif
