/*****************************************************************************/
/*                                                                           */
/*                       Random Polygon Generator (RPG)                      */
/*                                                                           */
/* Copyright (C)  T. Auer, M. Gschwandtner, M. Heimlich, M. Held   1994-2020 */
/*                                                                           */
/*****************************************************************************/

#include "eralloc.h"
#include "polyAVL.h"

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

int YVcompProc(caddr_t elem1, caddr_t elem2)
{
  t_polygon *sp1, *sp2;  
  int count, res;  

  sp1 = (t_polygon *) elem1;  
  sp2 = (t_polygon *) elem2;  
  
  res = 0;  
  count = 1;  

  /* compare the polygons */
  while ((res == 0) && (count <= BPsizeOf(sp1)))
    {
      if (BPgetPIndex(sp1, count) < BPgetPIndex(sp2, count))
	res = -1;  
      else if (BPgetPIndex(sp1, count) > BPgetPIndex(sp2, count))
	res =1;  

      count ++;  
    }

  return(res);  
}


void YVinitTree(t_polyAVL *aTree)
{
  ATinitTree(&(aTree->tree));  
  
}

void YVrecToArray(struct t_treeNode *aTree, t_yIncArray *yArray)
{
  if (aTree != NULL)
    {
      YIaddPoly(yArray, (t_polygon *)aTree->elem);  
      BPfree((t_polygon*)aTree->elem);  
      erfree(aTree->elem);  
      YVrecToArray(aTree->left, yArray);  
      YVrecToArray(aTree->right, yArray);  
      erfree(aTree);  
    }
}

void YVtoArray(t_polyAVL *aTree, t_yIncArray *yArray, t_pointArray *pArray)
{
  YVrecToArray(aTree->tree.root, yArray);  
}

void YVrecWrite(FILE *outFile, struct t_treeNode *aTree)
{
  if (aTree != NULL)
    {
      BPwriteToFile((t_polygon *)aTree->elem, outFile);  
      BPfree((t_polygon*)aTree->elem);  
      erfree(aTree->elem);  
      YVrecWrite(outFile, aTree->left);  
      YVrecWrite(outFile, aTree->right);  
      erfree(aTree);  
    }
}

void YVwritePoly(FILE *outFile, t_polyAVL *aTree)
{
  YVrecWrite(outFile, aTree->tree.root);  
}



void YVaddElem(t_polyAVL *aTree, t_polygon *aPoly)
{
  int res = FALSE;  
  t_polygon *compPoly;  
  
  compPoly = (t_polygon *) eralloc(sizeof(t_polygon));  
  BPinit(compPoly, BPsizeOf(aPoly));  
  
  /* copy the polygon to the allocated structure */
  BPcopyPoly(aPoly, compPoly);  
  
  /* add the polygon to the tree */
  res = ATaddElem(&(aTree->tree), (caddr_t)compPoly, YVcompProc);  
  
  if (!res)
    {
      /* free compPoly, since we have had this polygon already */
      BPfree(compPoly);  
      erfree(compPoly);  
    }
  /*}*/

}

void YVaddStarElem(t_polyAVL *aTree, t_starPoly *aPoly)
{
  YVaddElem(aTree, SPgetPoly(aPoly));  
}

int YVisEmpty(t_polyAVL *aTree)
{
  return(ATisEmpty(&(aTree->tree)));  
}




