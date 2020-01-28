/*****************************************************************************/
/*                                                                           */
/*                       Random Polygon Generator (RPG)                      */
/*                                                                           */
/* Copyright (C)  T. Auer, M. Gschwandtner, M. Heimlich, M. Held   1994-2020 */
/*                                                                           */
/*****************************************************************************/

#include "basicAVL.h"
#include <sys/types.h>

#ifndef __INDEXAVL_H_
#define __INDEXAVL_H_

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
} t_indexAVL;  

/********************************************************************/
/*                                                                  */
/* Procedures and functions                                         */
/*                                                                  */
/********************************************************************/

int ITcompProc(caddr_t elem1, caddr_t elem2);  

int ITisEmpty(t_indexAVL *aTree);  

void ITinitTree(t_indexAVL *aTree);  

void ITfreeTElem(t_indexAVL *aTree);  

void ITfreeTree(t_indexAVL *aTree);  

void ITaddElem(t_indexAVL *aTree, int newIndex);  

int ITrandomDelGet(t_indexAVL *aTree);  

#endif


