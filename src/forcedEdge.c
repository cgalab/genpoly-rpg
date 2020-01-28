/*****************************************************************************/
/*                                                                           */
/*                       Random Polygon Generator (RPG)                      */
/*                                                                           */
/* Copyright (C)  T. Auer, M. Gschwandtner, M. Heimlich, M. Held   1994-2020 */
/*                                                                           */
/*****************************************************************************/

#include "basicDefs.h"
#include "forcedEdge.h"

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
int FEedgeIndex(int pIndex1, int pIndex2, int nrOfPoints)
{
  int index;  
  int i1, i2;  
  
  i1 = MIN(pIndex1, pIndex2);  
  i2 = MAX(pIndex1, pIndex2);  
  
  index = (2*nrOfPoints - i1)*(i1 - 1)/2 + i2 - i1;  
  
  return(index);  
}

void initFEdge(t_forcedEdges *fEdge, int nrOfPoints)
{
  int nrOfLines;  
  int count;  
  
  nrOfLines = ((nrOfPoints*nrOfPoints) - nrOfPoints)/2 ;  
  
  IAinit(&(fEdge->pointForced), nrOfPoints);  
  IAinit(&(fEdge->forcedEdges), nrOfLines);  

  for (count=1;  count<=nrOfPoints;  count++)
    IAsetInt(&(fEdge->pointForced), count, 0);  

  for (count=1;  count<=nrOfLines;  count++)
    IAsetInt(&(fEdge->forcedEdges), count, nrOfPoints+1);  
}

void freeFEdge(t_forcedEdges *fEdge)
{
  IAfree(&(fEdge->pointForced));  
  IAfree(&(fEdge->forcedEdges));  
}

int nrOfForced(t_forcedEdges *fEdge, int pIndex)
{
  return(IAgetInt(&(fEdge->pointForced), pIndex));  
}


/* force the edge if possible, return false if the 
   there exist already 2 forced edges for either 
   pIndex1 or pIndex2 */
int forceEdge(t_forcedEdges *fEdge, t_edgeIArray *curForced, int pIndex1, 
	      int pIndex2, int curStep)
{
  int index;  
  int result = TRUE;  
  int nrOfPoints;  

  nrOfPoints = IAsizeOf(&(fEdge->pointForced));  
  
  index = FEedgeIndex(pIndex1, pIndex2, nrOfPoints);  
  
  if (IAgetInt(&(fEdge->forcedEdges), index) > nrOfPoints)
    {
      if ((IAgetInt(&(fEdge->pointForced), pIndex1) == 2) ||
	  (IAgetInt(&(fEdge->pointForced), pIndex2) == 2))
	result = FALSE;  
      else
	{
	  IAinc(&(fEdge->pointForced), pIndex1);  
	  IAinc(&(fEdge->pointForced), pIndex2);  
	  IAsetInt(&(fEdge->forcedEdges), index, curStep);  
	  EAaddEdge(curForced, pIndex1, pIndex2);  
	}   
    }
  return(result);  
}

/* check whether there exists a forced edge from orig
   to any other edge than used, if yes, return that edge */
int forcedEdge(t_forcedEdges *fEdge, int orig, t_intArray *usedPoints)
{
  int index, count;  
  int nrOfPoints;  
  int result = -1;  

  nrOfPoints = IAsizeOf(&(fEdge->pointForced));  
  
  for (count=1;  count<=nrOfPoints;  count++)
    if ((count != orig) && !IAgetInt(usedPoints, count))
      {
	index =  FEedgeIndex(orig, count, nrOfPoints);  
	if (IAgetInt(&(fEdge->forcedEdges), index) <= nrOfPoints)
	  result = count;  
      }
  return(result);  
}

int isForced(t_forcedEdges *fEdge, int pIndex1, int pIndex2)
{
  int index;  
  int nrOfPoints;  
  int result;  

  nrOfPoints = IAsizeOf(&(fEdge->pointForced));  
  
  index =  FEedgeIndex(pIndex1, pIndex2, nrOfPoints);  
  result = (IAgetInt(&(fEdge->forcedEdges), index) <= nrOfPoints);  

  return(result);  
}

int unforce(t_forcedEdges *fEdge, t_edgeIArray *curForced, int curStep)
{
  int count;  
  int pIndex1, pIndex2;  
  int nrOfPoints;  
  int index;  
  
  EAfreezeArray(curForced);  
  nrOfPoints = IAsizeOf(&(fEdge->pointForced));  

  for (count=1;  count <= EAnrOfEdges(curForced);  count++)
    {
      EAgetEdge(curForced, count, &pIndex1, &pIndex2);  
      index = FEedgeIndex(pIndex1, pIndex2, nrOfPoints);  
      
      if (IAgetInt(&(fEdge->forcedEdges), index) == curStep)
	{	
	  /* mark edge as unused */
	  IAsetInt(&(fEdge->forcedEdges), index, nrOfPoints+1);  
	  
	  /* decrease the number of forced edges! */
	  IAdec(&(fEdge->pointForced), pIndex1);  
	  IAdec(&(fEdge->pointForced), pIndex2);  
	}
    }
  return(TRUE);  
}

