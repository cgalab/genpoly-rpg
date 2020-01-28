/*****************************************************************************/
/*                                                                           */
/*                       Random Polygon Generator (RPG)                      */
/*                                                                           */
/* Copyright (C)  T. Auer, M. Gschwandtner, M. Heimlich, M. Held   1994-2020 */
/*                                                                           */
/*****************************************************************************/

#include "pathTree.h"
#include "eralloc.h"
#include "pointArray.h"
#include <stdlib.h>

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

/********************************************************************/
/*                                                                  */
/* Procedures and functions                                         */
/*                                                                  */
/********************************************************************/

void PTinitTree(t_pathTree *aTree)
{
  aTree->tree = (t_pathTreeNode*)
    eralloc(sizeof(t_pathTreeNode));  
  aTree->curNode = NULL;  
  aTree->tree->pointIndex = 1;  
  aTree->tree->child = NULL;  
  aTree->tree->sibling = NULL;  
}

void freeNode(t_pathTreeNode *aNode)
{
  if (aNode != NULL)
  {
    freeNode(aNode->child);  
    freeNode(aNode->sibling);  
    erfree(aNode);  
  }
}


void PTfreeTree(t_pathTree *aTree)
{
  freeNode(aTree->tree);  
  aTree->tree = NULL;  
  aTree->curNode = NULL;  
}


int recMakeIncTree(t_pathTree *aTree, t_pathTreeNode *node, int refPoint, 
                   t_pointArray *anArray, 
		   int mode)
{
  int compValue = 0;  
  int result = TRUE;  

  if (mode == TOP)
    compValue = 1;  
  else if (mode == BOTTOM)
    compValue = -1;  
  else
    result = FALSE;  

  if (result)
    {
      while ((node->child != NULL) &&
	    (PAisOnLine(anArray, node->child->pointIndex, node->pointIndex, 
			refPoint) == compValue))
	{
	  recMakeIncTree(aTree, node->child, refPoint, anArray, mode);  
	  node->child = node->child->sibling;  
	}
      aTree->curNode->sibling = node;  
      aTree->curNode = node;  
    }

  return(result);  
}
 
int hugo;  

int makeIncTree(t_pathTree *aTree, t_pointArray *anArray, int pointIndex, 
		int mode)
{
  t_pathTreeNode auxNode;  
  t_pathTreeNode *newNode;  

  aTree->curNode = &auxNode;  
  
  newNode = (t_pathTreeNode*)
    eralloc(sizeof(t_pathTreeNode));  
  newNode->pointIndex = pointIndex;  
  newNode->sibling = NULL;  

  recMakeIncTree(aTree, aTree->tree, pointIndex, anArray, mode);  

  newNode->child = auxNode.sibling;  
  aTree->tree = newNode;  
  aTree->curNode = aTree->tree->child;  

  return(TRUE);   
}

int PTmakeIncTop(t_pathTree *aTree, t_pointArray *anArray, int pointIndex)
{
  return(makeIncTree(aTree, anArray, pointIndex, TOP));  
}

int PTmakeIncBottom(t_pathTree *aTree, t_pointArray *anArray, int pointIndex)
{
  return(makeIncTree(aTree, anArray, pointIndex, BOTTOM));  
}

int PTgetNextIndex(t_pathTree *aTree)
{
  int result;  

  if (aTree->curNode != NULL)
  {
    result = aTree->curNode->pointIndex;  
    aTree->curNode = aTree->curNode->sibling;  
  }
  else
    result = -1;  
 
  return(result);  
}



