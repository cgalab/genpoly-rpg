/*****************************************************************************/
/*                                                                           */
/*                       Random Polygon Generator (RPG)                      */
/*                                                                           */
/* Copyright (C)  T. Auer, M. Gschwandtner, M. Heimlich, M. Held   1994-2020 */
/*                                                                           */
/*****************************************************************************/

#include "pointArray.h"

#ifndef __PATHTREE_H_
#define __PATHTREE_H_

/********************************************************************/
/*                                                                  */
/* Constants and the like                                           */
/*                                                                  */
/********************************************************************/

#define TOP	1
#define BOTTOM	2


/********************************************************************/
/*                                                                  */
/* Data Types                                                       */
/*                                                                  */
/********************************************************************/

struct t_pathTreeNodeStruct{
  int pointIndex;  
  struct t_pathTreeNodeStruct *child;  
  struct t_pathTreeNodeStruct *sibling;  
};  

typedef struct t_pathTreeNodeStruct t_pathTreeNode;  

typedef struct{
  t_pathTreeNode *tree;  
  t_pathTreeNode *curNode;  
} t_pathTree;  

/********************************************************************/
/*                                                                  */
/* Procedures and functions                                         */
/*                                                                  */
/********************************************************************/

void PTinitTree(t_pathTree *aTree);  

void PTfreeTree(t_pathTree *aTree);  

int PTmakeIncTop(t_pathTree *aTree, t_pointArray *anArray, int pointIndex);  

int PTmakeIncBottom(t_pathTree *aTree, t_pointArray *anArray, int pointIndex);  

int PTgetNextIndex(t_pathTree *aTree);  

#endif
