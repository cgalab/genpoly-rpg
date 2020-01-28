/*****************************************************************************/
/*                                                                           */
/*                       Random Polygon Generator (RPG)                      */
/*                                                                           */
/* Copyright (C)  T. Auer, M. Gschwandtner, M. Heimlich, M. Held   1994-2020 */
/*                                                                           */
/*****************************************************************************/

#include <stdlib.h>
#include <sys/types.h>

#ifndef __BASICAVL_H_
#define __BASICAVL_H_

/********************************************************************/
/*                                                                  */
/* Constants and the like                                           */
/*                                                                  */
/********************************************************************/
#define AT_LEFT              1
#define AT_BAL               0
#define AT_RIGHT            -1


/* for randomGet we need a seed for deciding whether we take the current
   element, or search in the right resp. left subtree */
#define SPLIT_BASE	     0.4
/********************************************************************/
/*                                                                  */
/* Data Types                                                       */
/*                                                                  */
/********************************************************************/

struct t_treeNode {
  caddr_t elem;  
  int balance;  
  struct t_treeNode *left, *right;  
};  

typedef struct treeNode *treeNodeP;  


typedef struct {
  struct t_treeNode *root;  
} t_AVLtree;  


/********************************************************************/
/*                                                                  */
/* Procedures and functions                                         */
/*                                                                  */
/********************************************************************/




void ATinitTree(t_AVLtree *aTree);  

int ATisEmpty(t_AVLtree *aTree);  

caddr_t ATsearch(t_AVLtree *aTree, caddr_t elem, int compProc(caddr_t, caddr_t));  

caddr_t ATrandomGet(t_AVLtree *aTree);  

caddr_t ATviewMax(t_AVLtree *aTree);  

caddr_t ATviewMin(t_AVLtree *aTree);  

int ATaddElem(t_AVLtree *aTree, caddr_t elem, int compProc(caddr_t, caddr_t));  

void ATdelElem(t_AVLtree *aTree, caddr_t elem, int compProc(caddr_t, caddr_t), 
	       void delProc(void *));  

/* free the tree, but don't free the data elements */
void ATfreeTree(t_AVLtree *aTree);  

/* free both the tree and the data elements it contains */
void ATfreeTElem(t_AVLtree *aTree, void delProc(void*));  

#endif



