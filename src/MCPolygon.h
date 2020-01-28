/*****************************************************************************/
/*                                                                           */
/*                       Random Polygon Generator (RPG)                      */
/*                                                                           */
/* Copyright (C)  T. Auer, M. Gschwandtner, M. Heimlich, M. Held   1994-2020 */
/*                                                                           */
/*****************************************************************************/

#include "basicPoly.h"

#ifndef __MCPOLYGON_H_
#define __MCPOLYGON_H_

/* Multiply connected Polygons */
void MCPolygon2(t_polygon *aPolygon, t_pointArray *pArray, int hcount);  
void MCsmoothPoly(t_pointList *aList, t_pointArray *pArray);  
void MCreset();  
void MCclear();  
int MCsavePoly(char* filename, t_pointArray *pArray);  
void MCwriteVoronoi(char *fileName, t_pointArray *pArray);  
void MCipeWrite(FILE *ipeFile, t_pointArray *pArray);  
t_polygon* MCgetNextPolygon();  

extern int pcount;  
extern t_polygon *polygons;  
#endif

