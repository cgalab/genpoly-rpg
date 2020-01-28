/*****************************************************************************/
/*                                                                           */
/*                       Random Polygon Generator (RPG)                      */
/*                                                                           */
/* Copyright (C)  T. Auer, M. Gschwandtner, M. Heimlich, M. Held   1994-2020 */
/*                                                                           */
/*****************************************************************************/

#include "basicAVL.h"
#include <sys/types.h>

#ifndef __TRIANGAVL_H_
#define __TRIANGAVL_H_

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
  int pIndex1, pIndex2;  
  int triangIndex1, triangIndex2;  
  int marker;  
} t_triangEdge;  

typedef struct {
  t_AVLtree tree;  
} t_triangAVL;  

/********************************************************************/
/*                                                                  */
/* Procedures and functions                                         */
/*                                                                  */
/********************************************************************/

int TAcompProc(caddr_t elem1, caddr_t elem2);  

int TAisEmpty(t_triangAVL *aTree);  

void TAinitTree(t_triangAVL *aTree);  

void TAfreeTree(t_triangAVL *aTree);  

void TAfreeTElem(t_triangAVL *aTree);  

void TAaddElem(t_triangAVL *aTree, int pIndex1, int pIndex2, int triangIndex, 
	       int marker);  

void TAdelElem(t_triangAVL *aTree, int pIndex1, int pIndex2);  

t_triangEdge *TAsearch(t_triangAVL *aTree, int pIndex1, int pIndex2);  

t_triangEdge *TArandomGet(t_triangAVL *aTree);  

#endif





