/*****************************************************************************/
/*                                                                           */
/*                       Random Polygon Generator (RPG)                      */
/*                                                                           */
/* Copyright (C)  T. Auer, M. Gschwandtner, M. Heimlich, M. Held   1994-2020 */
/*                                                                           */
/*****************************************************************************/

#include "edgeIncArray.h"
#include "basicGeom.h"
#include "geomHash.h"

#include <stdlib.h>
#include <math.h>

/********************************************************************/
/*                                                                  */
/* Constants and the like                                           */
/*                                                                  */
/********************************************************************/



/********************************************************************/
/*                                                                  */
/* Procedures and functions                                         */
/*                                                                  */
/********************************************************************/

void EAinitArray(t_edgeIArray *anArray, int initialSize)
{ 
   IIinitArray(&(anArray->pIndex1), initialSize);  
   IIinitArray(&(anArray->pIndex2), initialSize);    
}

void EAinitArray2(t_edgeIArray *anArray, int initialSize)
{ 
   int i, size, size2;

   anArray->gridSize = size = (int)(0.5*sqrt(initialSize))+10;  
   size2 = size * size;

   //printf ("SECT_GRID_SIZE: %i\n", anArray->gridSize);  
   allocSectorArray(anArray->gridSize);  
   anArray->geomHash=(struct hashSector *)malloc(size2*sizeof(hashSector));  
   anArray->dirtySectors =(struct hashSector **)malloc(size2*sizeof(hashSector *));  
   for (i = 0;  i < size2;  i++) {
      NLInit(&(anArray->geomHash[i].lines));  
      anArray->geomHash[i].sectorNumber = i;  
      anArray->geomHash[i].dirty = 0;  
   }
   anArray->numDirty = 0;  
   //NLInit(anArray->dirtySectors);  
   
   VIinitArray(&(anArray->lines), initialSize);  
   
   IIinitArray(&(anArray->pIndex1), initialSize);  
   IIinitArray(&(anArray->pIndex2), initialSize);  
}



void EAgetEdge(t_edgeIArray *anArray, int index, int *pIndex1, int *pIndex2)
{
   (*pIndex1) = IIgetInt(&(anArray->pIndex1), index);  
   (*pIndex2) = IIgetInt(&(anArray->pIndex2), index);  
}



void EAsetEdge2(t_edgeIArray *anArray, t_pointArray *pArray,
                int index, int pIndex1, int pIndex2)
{
   GHsetLine(anArray->dirtySectors, &anArray->numDirty, 
             (hashSector *) anArray->geomHash, 
             (nodeList *) &(anArray->dirtySectors), 
             (lineContainer *) VIgetVoid(&(anArray->lines), index), 
             index, PAgetPoint(pArray, pIndex1), 
             PAgetPoint(pArray, pIndex2), pIndex1, pIndex2, 
             anArray->gridSize);  	
   EAsetEdge (anArray, index, pIndex1, pIndex2);  
}


/* Validate that the Edge still exists */

int EAvalidateEdge(t_edgeIArray *anArray, int lineNum, 
                   int pIndex1, int pIndex2) 
{
   int i1, i2;  
   i1 = IIgetInt(&(anArray->pIndex1), lineNum);  
   i2 = IIgetInt(&(anArray->pIndex2), lineNum);  
   //debug//	printf ("Validate Edge: %i (%i, %i == %i, %i)  ", lineNum, pIndex1, pIndex2, i1, i2);  
   
   if (MIN(pIndex1, pIndex2) == MIN(i1, i2) && 
       MAX(pIndex1, pIndex2) == MAX(i1, i2)) {    
      //debug// 		printf ("[VALID]\n");  
      return 1;  
   } 
   else {
      //debug// 		printf ("[INVALID]\n");  
      return 0;  
   }
}



void EAsetEdge(t_edgeIArray *anArray, int index, int pIndex1, int pIndex2)
{
	
  IIsetInt(&(anArray->pIndex1), index, pIndex1);  
  IIsetInt(&(anArray->pIndex2), index, pIndex2);  
}


nodeList *EAgetPossibleIsects(t_edgeIArray *anArray, int index) 
{
   return GHgetPossibleIntersects (anArray->geomHash, 
                                   (lineContainer *)VIgetVoid(&(anArray->lines), index));  
}



void EAaddEdge2(t_edgeIArray *anArray, t_pointArray *pArray,
                int pIndex1, int pIndex2)
{
   int index;  
   
   struct lineContainer *tmp;  
   index = VIsizeOf(&(anArray->lines));  
   tmp = LCNew(index);  
   VIaddVoid(&(anArray->lines), (void *)tmp);  
   
   
   GHsetLine(anArray->dirtySectors, &anArray->numDirty, 
             (hashSector *)anArray->geomHash, 
             (nodeList *)&(anArray->dirtySectors), 
             (lineContainer *) tmp, anArray->pIndex1.curNrOfPoints, 
             PAgetPoint(pArray, pIndex1), 
             PAgetPoint(pArray, pIndex2), 
             pIndex1, pIndex2, anArray->gridSize);  	
   
   EAaddEdge(anArray, pIndex1, pIndex2);  	
   
}

int EAhasDirtySectors(t_edgeIArray *anArray) {
   
   return anArray->numDirty;  
   
}


int EAisDirty(t_edgeIArray *anArray, int index) {
   return anArray->dirtySectors[index]->dirty;  	
}


nodeList *EAgetLinesOfDirtySector(t_edgeIArray *anArray, int index) 
{  
   return &(anArray->dirtySectors[index]->lines);  	
}

void EAcleanSector(t_edgeIArray *anArray, int index) 
{   
   if (anArray->numDirty > 0) {
      anArray->dirtySectors[index]->dirty = 0;  
      anArray->dirtySectors[index] = anArray->dirtySectors[anArray->numDirty-1];  	
      anArray->numDirty--;  
   }   
}


void EAaddEdge(t_edgeIArray *anArray, int pIndex1, int pIndex2)
{
   IIaddInt(&(anArray->pIndex1), MIN(pIndex1, pIndex2));  
   IIaddInt(&(anArray->pIndex2), MAX(pIndex1, pIndex2));  
}


int EAnrOfEdges(t_edgeIArray *anArray)
{
   return(IIsizeOf(&(anArray->pIndex1)));  
}


void EAfreeArray(t_edgeIArray *anArray)
{
   
   IIfreeArray(&(anArray->pIndex1));  
   IIfreeArray(&(anArray->pIndex2));  
}


void EAfreeArray2(t_edgeIArray *anArray)
{
   listNode *tmp, *node;  
   lineElement *ltmp, *lnode;  
   int i, size2;  
   
   size2 = anArray->gridSize*anArray->gridSize;
   for (i = 0;  i < size2;  i++) {
      tmp = NLGetIter(&(anArray->geomHash[i].lines));  
      while (tmp != NULL) {
         if (tmp->data != NULL) lineElement_free(tmp->data);  
         node=tmp->next;  
         NLFreeNode(tmp);  
         tmp=node;  	
      }  
   }
   
   tmp = NLget_FreeList();  
   while (tmp != NULL) {
      node=tmp->next;  
      
      NLFreeNode(tmp);  
      tmp=node;  	
   }  
   
   ltmp = getLineElement_FreeList();  
   while (ltmp != NULL) {
      
      lnode=ltmp->nextHook;  
      lineElement_free(ltmp);  			
      ltmp=lnode;  	
   }  
   
   resetLineElement_FreeList();  
   NLreset_FreeList();  
   freeSectorArray();  
   IIfreeArray(&(anArray->pIndex1));  
   IIfreeArray(&(anArray->pIndex2));  
}



/* reallocate to current size */
void EAfreezeArray(t_edgeIArray *anArray)
{
   IIfreezeArray(&(anArray->pIndex1));  
   IIfreezeArray(&(anArray->pIndex2));  
}
