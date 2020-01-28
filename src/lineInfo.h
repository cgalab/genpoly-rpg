/*****************************************************************************/
/*                                                                           */
/*                       Random Polygon Generator (RPG)                      */
/*                                                                           */
/* Copyright (C)  T. Auer, M. Gschwandtner, M. Heimlich, M. Held   1994-2020 */
/*                                                                           */
/*****************************************************************************/

#include "quadEdge.h"
#include "basicLine.h"
#include "basicDefs.h"
#include <sys/types.h>
#include <stdlib.h>

#ifndef __LINEINFO_H_
#define __LINEINFO_H_

/********************************************************************/
/*                                                                  */
/* Constants and the like                                           */
/*                                                                  */
/********************************************************************/

/* y-coordinates for sorting the lines */
#define COMP_MIN              MIN(X_MIN, -10.0)
#define COMP_MAX              MAX(X_MAX, 10.0)

/********************************************************************/
/*                                                                  */
/* Data Types                                                       */
/*                                                                  */
/********************************************************************/
typedef struct {
  t_line line;  
  t_edgeInfo *curEdge;  
  int lastTouched;  
} t_lInfo;  

typedef t_lInfo* t_lInfoP;  

/********************************************************************/
/*                                                                  */
/* Procedures and functions                                         */
/*                                                                  */
/********************************************************************/


int LIcompLines(t_lInfoP line1, t_lInfoP line2, double xcoord);  

/* qsort expects -1 if l1 < l2, this is NOT the behaviour we
   normally want (inconsistent with geometric considerations)! */
int LIqsortMinLines(const void *l1, const void *l2);  

int LIcompMinLines(caddr_t l1, caddr_t l2);  

int LIcompMaxLines(caddr_t l1, caddr_t l2);  

#endif








