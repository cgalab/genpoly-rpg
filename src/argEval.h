/*****************************************************************************/
/*                                                                           */
/*                       Random Polygon Generator (RPG)                      */
/*                                                                           */
/* Copyright (C)  T. Auer, M. Gschwandtner, M. Heimlich, M. Held   1994-2020 */
/*                                                                           */
/*****************************************************************************/

#include "calcPoly.h"

#ifndef __ARGEVAL_H_
#define __ARGEVAL_H_

/********************************************************************/
/*                                                                  */
/* Constants and the like                                           */
/*                                                                  */
/********************************************************************/

/* output formats */
#define STANDARD     1
#define IPE          2
#define DAT          3
#define LINE         4
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

int AEeval(int argc, char *argv[], int *nrOfPoints, int *nrOfPolys, 
           int *format, int *seed, int *smooth, enum t_calcType *algo, 
           char *inFile, char *outFile, int *analysis, int *sinuosity, 
           int *angRes, int *lenRes, int *slopeRes, int *auxParam, 
           int *cluster, int *nholes);  


#endif








