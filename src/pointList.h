/*****************************************************************************/
/*                                                                           */
/*                       Random Polygon Generator (RPG)                      */
/*                                                                           */
/* Copyright (C)  T. Auer, M. Gschwandtner, M. Heimlich, M. Held   1994-2020 */
/*                                                                           */
/*****************************************************************************/

#include "basicGeom.h"
#include <stdio.h>

#ifndef __POINTLIST_H_
#define __POINTLIST_H_


/********************************************************************/
/*                                                                  */
/* Constants and the like                                           */
/*                                                                  */
/********************************************************************/

/* maximum number of points to generate without using arrays */
#define MAX_RAND 50


/********************************************************************/
/*                                                                  */
/* Data Types                                                       */
/*                                                                  */
/********************************************************************/

struct t_pointListElem{
   t_point datum;  
   struct t_pointListElem *next;  
   struct t_pointListElem *prev;  
};  

typedef struct {
   struct t_pointListElem *head;  
   struct t_pointListElem *tail;  
   struct t_pointListElem *current;  
} t_pointList;  


/********************************************************************/
/*                                                                  */
/* Procedures and functions                                         */
/*                                                                  */
/********************************************************************/

void PLsetCheck(int c);  

void PLinitList(t_pointList *aList);  

int PLaddPoint(t_pointList *aList, t_point aPoint);  

void PLgetMinMax(t_pointList *aList, t_point *minPoint, t_point *maxPoint);  

int PLaddRandomPoints(t_pointList *aList, int nrOfPoints);  

int PLaddRandomCicrlePoints(t_pointList *aList, int nrOfPoints);  

int PLaddClusterPoints(t_pointList *aList, int nrOfPoints);  

int PLdelPoint(t_pointList *aList, t_point aPoint);  

int PLdelEpsilonPoints(t_pointList *aList, t_point aPoint, 
                       double epsilon);  

int PLgetCurrentPoint(t_pointList *aList, t_point *aPoint);  

int PLresetList(t_pointList *aList);  

int PLcleanList(t_pointList *aList);  

int PLfreeList(t_pointList *aList);  

int PLreadFile(t_pointList *aList, char *fileName);  

int PLreadFromFile(t_pointList *aList, FILE *inputFile);  

int PLwriteFile(t_pointList *aList, char *fileName);  

int PLwriteToFile(t_pointList *aList, FILE *outputFile);  

int PLcountPoints(t_pointList *aList);  

int PLaddRandomCirclePoints(t_pointList *aList, int nrOfPoints);  

#endif
