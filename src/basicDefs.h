/*****************************************************************************/
/*                                                                           */
/*                       Random Polygon Generator (RPG)                      */
/*                                                                           */
/* Copyright (C)  T. Auer, M. Gschwandtner, M. Heimlich, M. Held   1994-2020 */
/*                                                                           */
/*****************************************************************************/

#include <stddef.h>
#include <math.h>

#ifndef __BASICDEF_H_
#define __BASICDEF_H_

/********************************************************************/
/*                                                                  */
/* Constants and the like                                           */
/*                                                                  */
/********************************************************************/
/* define TRUE and FALSE if they have not been already defined */
#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE !TRUE
#endif

#define RPG_UNDEFINED 	-1
#define RPG_USED		-2	

/* define a maximum and a minimum operator */
#ifndef MAX
#define MAX(x, y) ((x)>(y) ? (x) : (y))
#endif
#ifndef MIN
#define MIN(x, y) ((x)>(y) ? (y) : (x))
#endif


/* define a signum operator */
#define SIGN(x) ((x)==0.0 ? 0 : ((x)>0 ? 1 : -1))

/* Extremas for the points */
/* all points are within [0, 1]^2 */
/* NOTE if either X_MAX = 0 or Y_MAX = 0, initialization in initList()
   has to be changed! */
#define X_MIN 0.0
#define Y_MIN 0.0
#define X_MAX 1.0
#define Y_MAX 1.0

/* define 2 values outside [X_MIN, X_MAX] x [Y_MIN, Y_MAX] */
#define X_OUT -X_MAX
#define Y_OUT -Y_MAX

/* maximum value the random number generator will produce: */
/*  (2^31)-1 for lrand48(), see man page(man lrand48) */
#define RANDOM_MAX		2147483647
#define SEED			0

#endif
