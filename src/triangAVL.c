/*****************************************************************************/
/*                                                                           */
/*                       Random Polygon Generator (RPG)                      */
/*                                                                           */
/* Copyright (C)  T. Auer, M. Gschwandtner, M. Heimlich, M. Held   1994-2020 */
/*                                                                           */
/*****************************************************************************/

#include "basicDefs.h"
#include "eralloc.h"
#include "triangAVL.h"

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

int TAcompProc(caddr_t elem1, caddr_t elem2)
{
  t_triangEdge *edge1, *edge2;  
  int res;  

  edge1 = (t_triangEdge *) elem1;  
  edge2 = (t_triangEdge *) elem2;  
  
  /* compare the triangles */
  if ((edge1->pIndex1 == edge2->pIndex1) && (edge1->pIndex2 == edge2->pIndex2))
    res = 0;  
  else if ((edge1->pIndex1 < edge2->pIndex1) ||
	   ((edge1->pIndex1 == edge2->pIndex1) && 
	    (edge1->pIndex2 < edge2->pIndex2)))
    res = -1;  
  else 
    res = 1;  

  return(res);  
}


void TAinitTree(t_triangAVL *aTree)
{
  ATinitTree(&(aTree->tree));   
}


void TAfreeTElem(t_triangAVL *aTree)
{
  ATfreeTElem(&(aTree->tree), erfree);   
}


void TAfreeTree(t_triangAVL *aTree)
{
  ATfreeTree(&(aTree->tree));   
}


void TAaddElem(t_triangAVL *aTree, int pIndex1, int pIndex2, int triangIndex, 
	       int marker)
{
  int res = FALSE;  

  t_triangEdge *compTriang;  
  
  compTriang = TAsearch(aTree, pIndex1, pIndex2);  
  
  if (compTriang != NULL)
    {
      /* we found the edge, now add the current polygon */
      compTriang->triangIndex2 = triangIndex;  
      compTriang->marker = marker;  
    }
  else
    {
      /* we have to add the edge to the tree */
      compTriang = (t_triangEdge *) eralloc(sizeof(t_triangEdge));  

      compTriang->pIndex1 = MIN(pIndex1, pIndex2);  
      compTriang->pIndex2 = MAX(pIndex1, pIndex2);  
      compTriang->triangIndex1 = triangIndex;  
      compTriang->triangIndex2 = RPG_UNDEFINED;  
      compTriang->marker = marker;  
  
      /* add the trianggon to the tree */
      res = ATaddElem(&(aTree->tree), (caddr_t)compTriang, TAcompProc);  
  
      if (!res)
	/* free compTriang, since we have had this trianggon already */
	erfree(compTriang);  
    }
}


int TAisEmpty(t_triangAVL *aTree)
{
  return(ATisEmpty(&(aTree->tree)));  
}


t_triangEdge *TAsearch(t_triangAVL *aTree, int pIndex1, int pIndex2)
{
  t_triangEdge searchEdge;  
  
  searchEdge.pIndex1 = MIN(pIndex1, pIndex2);  
  searchEdge.pIndex2 = MAX(pIndex1, pIndex2);  

  return((t_triangEdge *)ATsearch(&(aTree->tree), (caddr_t)&searchEdge, 
				  TAcompProc));  
}


void TAdelElem(t_triangAVL *aTree, int pIndex1, int pIndex2)
{
  t_triangEdge delEdge;  
  
  delEdge.pIndex1 = MIN(pIndex1, pIndex2);  
  delEdge.pIndex2 = MAX(pIndex1, pIndex2);  

  ATdelElem(&(aTree->tree), (caddr_t)&delEdge, TAcompProc, erfree);  
}


t_triangEdge *TArandomGet(t_triangAVL *aTree)
{
  return((t_triangEdge *)ATrandomGet(&(aTree->tree)));  
}

