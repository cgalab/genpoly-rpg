/*****************************************************************************/
/*                                                                           */
/*                       Random Polygon Generator (RPG)                      */
/*                                                                           */
/* Copyright (C)  T. Auer, M. Gschwandtner, M. Heimlich, M. Held   1994-2020 */
/*                                                                           */
/*****************************************************************************/

#ifndef __BASICGEOM_H_
#define __BASICGEOM_H_

#include "basicDefs.h"
/********************************************************************/
/*                                                                  */
/* Constants and the like                                           */
/*                                                                  */
/********************************************************************/

/* define an initial delta value for checking equal to zero. */
/* any number lying in the delta environment of zero yields */
/* TRUE, all others FALSE */
#define DELTA 1.0e-14


/********************************************************************/
/*                                                                  */
/* Data Types                                                       */
/*                                                                  */
/********************************************************************/

typedef struct {
   double x;  
   double y;  
   double x_ori;
   double y_ori;
} t_point;  

extern t_point BGdefaultPoint;  

/********************************************************************/
/*                                                                  */
/* Procedures and functions                                         */
/*                                                                  */
/********************************************************************/
int randomInt(int min, int max);  

double randomDouble();  

unsigned long randomULong(unsigned long min, unsigned long max);  

t_point randomPoint();  

/* normalize a vector, such that |vector| = 1, where || is the
   euclidean norm */
void normVec(t_point *vector);  

/* test if point lies within [X_MIN, X_MAX]x[Y_MIN, Y_MAX] */
int validPoint(t_point aPoint);  

double lineLength(t_point linePoint1, t_point linePoint2);  

/* test if point1 < point2, i.e. either (x1<x2) or (x1=x2 and y1<y2)*/
/* NO TEST ON EQUALITY! */
int compPoints(t_point point1, t_point point2);  

/* test FOR EQUALITY */
int equalPoints(t_point point1, t_point point2);  

/* compSwapPoints swaps point1 and point2 if !compPoints(point1, point2), 
   i.e. point2 < point1 */
void compSwapPoints(t_point *point1, t_point *point2);  

/* swap 2 points */
void swapPoints(t_point *point1, t_point *point2);  

/* test where aPoint lies with respect to the oriented line segment 
   (linePoint1, linePoint2): left(result:1), right(-1) or on the
   segment(0) */
int isOnLine(t_point linePoint1, t_point linePoint2, t_point aPoint);  

/* test if the line segments (line1point1, line1point2) and 
   (line2point1, line2point1) intersect */
int isectSegments(t_point line1point1, t_point line1point2, 
               t_point line2point1, t_point line2point2);  

/*****************/
/* W A R N I N G */
/*****************/
/* DO NOT USE THE FOLLOWING ROUTINES UNLESS ALL PARAMETERS WILL BE PASSED
   IN THE SAME ORDER IF THE ROUTINE IS CALLED MORE THAN ONCE! */

/* test where aPoint lies with respect to the oriented line segment 
   (linePoint1, linePoint2): left(result:1), right(-1) or on the
   segment(0) */
int isOnOrderedLine(t_point linePoint1, t_point linePoint2, 
                    t_point aPoint);  

/* test if the line segments (line1point1, line1point2) and 
   (line2point1, line2point1) intersect*/
/* DO NOT USE THIS ROUTINE UNLESS THE FOUR POINTA WILL ALWAS APPEAR 
   IN THE SAME ORDER FOR THE TWO SEGMENTS! */
int isectOrderedSegments(t_point line1point1, t_point line1point2, 
                      t_point line2point1, t_point line2point2);  

/* set the delta for comparisons */
int setDelta(double delta);  

/* get the current delat value */
double getDelta();  

/* test for less than delta (compare with constant DELTA) */
int deltaAbs(double compVal);  

/* test for less than delta (compare with getDelta()) */
int deltaVal(double compVal);  

#endif
