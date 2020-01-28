/*****************************************************************************/
/*                                                                           */
/*                       Random Polygon Generator (RPG)                      */
/*                                                                           */
/* Copyright (C)  T. Auer, M. Gschwandtner, M. Heimlich, M. Held   1994-2020 */
/*                                                                           */
/*****************************************************************************/

#include "basicDefs.h"
#include "eralloc.h"
#include "indexAVL.h"

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


int ITcompProc(caddr_t elem1, caddr_t elem2)
{
  int *index1, *index2;  
  int res;  

  index1 = (int *) elem1;  
  index2 = (int *) elem2;  
  
  /* compare the indices */
  if (*index1 == *index2)
    res = 0;  
  else if (*index1 < *index2)
    res = -1;  
  else 
    res = 1;  

  return(res);  
}


void ITinitTree(t_indexAVL *aTree)
{
  ATinitTree(&(aTree->tree));   
}


void ITfreeTElem(t_indexAVL *aTree)
{
  ATfreeTElem(&(aTree->tree), erfree);   
}


void ITfreeTree(t_indexAVL *aTree)
{
  ATfreeTree(&(aTree->tree));   
}


void ITaddElem(t_indexAVL *aTree, int newIndex)
{
  int res = FALSE;  

  int *compIndex;  
  
  compIndex = (int *) eralloc(sizeof(int));  
  *compIndex = newIndex;  
      
  /* add the index to the tree */
  res = ATaddElem(&(aTree->tree), (caddr_t)compIndex, ITcompProc);  
  
  if (!res)
    /* free compIndex, since we have had this index already */
    erfree(compIndex);  
}



int ITisEmpty(t_indexAVL *aTree)
{
  return(ATisEmpty(&(aTree->tree)));  
}


int ITrandomDelGet(t_indexAVL *aTree)
{
  int result;  

  /* first, get the element from the tree */
  result = *(int *)ATrandomGet(&(aTree->tree));  
  
  /* second, delete the element from the tree */
  ATdelElem(&(aTree->tree), (caddr_t)&result, ITcompProc, erfree);  

  return(result);  
}


