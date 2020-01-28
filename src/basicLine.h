/*****************************************************************************/
/*                                                                           */
/*                       Random Polygon Generator (RPG)                      */
/*                                                                           */
/* Copyright (C)  T. Auer, M. Gschwandtner, M. Heimlich, M. Held   1994-2020 */
/*                                                                           */
/*****************************************************************************/

#include "pointArray.h"
#include "basicGeom.h"

#ifndef __BASICLINE_H_
#define __BASICLINE_H_

/********************************************************************/
/*                                                                  */
/* Constants and the like                                           */
/*                                                                  */
/********************************************************************/

/* espilon for multiplying the scalar for the intersection point p
   such that 
   BNisOnLine(l1, p) = TRUE and
   BNisOnLine(l2, p) = TRUE */
#define FACTOR 1.1  

/* define sorting distance: check whether we have parallel 
   lines (for division) */
#define PARDELTA 1.0e-20


/********************************************************************/
/*                                                                  */
/* Data Types                                                       */
/*                                                                  */
/********************************************************************/

typedef struct {
  int pIndex1, pIndex2;  
  t_point point1, point2;  /* the two points which define the line */
  double a, b, c;           /* for HNF, we have ax+by+c =0 for any point */
                         /* point (x, y) on the line */
} t_line;  

typedef t_line *t_lineP;  

/********************************************************************/
/*                                                                  */
/* Procedures and functions                                         */
/*                                                                  */
/********************************************************************/

/* this will create a line such that p1 < p2! */
void BNcreatePLine(t_line *aLine, int pIndex1, int pIndex2, t_pointArray *pArray);  


/* BNcreateLine DOES NOT sort the points! */
void BNcreateLine(t_line *aLine, t_point point1, t_point point2);  

void BNcreateDummy(t_line *aLine);  

t_line BNminLine(t_line *line1, t_line *line2);  

t_line BNmaxLine(t_line *line1, t_line *line2);  

int BNisectLines(t_line *line1, t_line *line2, t_point *isectPoint);  

/* for a given x value, calculate y s.t. the point lies on the line, 
   return y, return TRUE, if such a point exists, else FALSE 
   (we need this for handling vertical lines!) */

int BNsetYcoord(t_line *aLine, t_point *aPoint);  

int BNsetXcoord(t_line *aLine, t_point *aPoint);  

int BNisOnLine(t_line *aLine, t_point *aPoint);  

/* left = 1, right = -1, on = 0 */
int BNisOnSide(t_line *aLine, t_point *aPoint);  

double BNdistance(t_line *aLine, t_point *aPoint);  

int BNisVertical(t_line *aLine);  

void BNcopyLine(t_line *from, t_line *to);  

t_point BNgetPoint1(t_line *aLine);  

t_point BNgetPoint2(t_line *aLine);  

int BNgetPIndex1(t_line *aLine);  

void BNsetPIndex1(t_line *aLine, int newIndex);  

int BNgetPIndex2(t_line *aLine);  

int BNisSame(t_line *line1, t_line *line2);  

#endif
