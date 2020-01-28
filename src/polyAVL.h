/*****************************************************************************/
/*                                                                           */
/*                       Random Polygon Generator (RPG)                      */
/*                                                                           */
/* Copyright (C)  T. Auer, M. Gschwandtner, M. Heimlich, M. Held   1994-2020 */
/*                                                                           */
/*****************************************************************************/

#include "basicPoly.h"
#include "starPoly.h"
#include "pointArray.h"
#include "polyIncArray.h"
#include "basicAVL.h"
#include <sys/types.h>

#ifndef __NEWSPOLYAVL_H_
#define __NEWSPOLYAVL_H_

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
  t_AVLtree tree;  
} t_polyAVL;  

/********************************************************************/
/*                                                                  */
/* Procedures and functions                                         */
/*                                                                  */
/********************************************************************/

int YVcompProc(caddr_t elem1, caddr_t elem2);  

void YVinitTree(t_polyAVL *aTree);  

void YVaddElem(t_polyAVL *aTree, t_polygon *aPoly);  

void YVaddStarElem(t_polyAVL *aTree, t_starPoly *aPoly);  

void YVtoArray(t_polyAVL *aTree, t_yIncArray *yArray, t_pointArray *pArray);  

void YVwritePoly(FILE *outFile, t_polyAVL *aTree);  

int YVisEmpty(t_polyAVL *aTree);  

#endif


