/*****************************************************************************/
/*                                                                           */
/*                       Random Polygon Generator (RPG)                      */
/*                                                                           */
/* Copyright (C)  T. Auer, M. Gschwandtner, M. Heimlich, M. Held   1994-2020 */
/*                                                                           */
/*****************************************************************************/

#include <stdlib.h>

#include "geomHash.h"
#include "edgeIncArray.h"

static struct lineElement *lineElement_freelist = NULL;  // Freie lineElemente
static int maxlineElements_inUse = 0;  
static int maxUsed = 0;  
static int *sectors=NULL;  


int lineSects(double x1, double y1, double x2, double y2, int *sectors, 
              int sect_size) 
{
   double dx, dy;  
   int xsector, ysector, xend, yend;  
   double xg, yg, det;  
   int quadrant, steps = 0, running;  
   dx = x2 - x1;  
   dy = y2 - y1;  
   
   if ((x1 >= 0.0)  &&  (x1 <= 1.0)  &&  (x2 >= 0.0)  &&  (x2 <= 1.0)  && 
       (y1 >= 0.0)  &&  (y1 <= 1.0)  &&  (y2 >= 0.0)  &&  (y2 <= 1.0)) {
      xsector = (int)(x1*sect_size);  
      ysector = (int)(y1*sect_size);  
      xend = (int)(x2*sect_size);  
      yend = (int)(y2*sect_size);  
      int tmp;  
	  
      quadrant = getQuadrant (dx, dy);  
      
      tmp = mapCoordinate(xsector, ysector, sect_size);  
      if ((tmp >= 0)   &&  (tmp < sect_size*sect_size))  sectors[steps++] = tmp;  
      
      running=checkEnd(xsector, ysector, xend, yend, quadrant);   
      
      while (running) {
         
         getGridPoint(quadrant, xsector, ysector, &xg, &yg, sect_size);  
         
         det = det3Point (x1, y1, xg, yg, x2, y2);  
		 
         if (quadrant == 0) {
            if (det > DELTA)
               ysector = ysector -1;  
            else if (det < -DELTA)
               xsector = xsector -1;  
            else {
               xsector = xsector -1;  
               ysector = ysector -1;  
               tmp = mapCoordinate(xsector+1, ysector, sect_size);  
               if ( tmp>=0 && tmp < sect_size*sect_size) sectors[steps++] = tmp;  
               tmp = mapCoordinate(xsector, ysector+1, sect_size);  
               if ( tmp>=0 && tmp < sect_size*sect_size) sectors[steps++] = tmp;  
            }
         }
         else if (quadrant == 1) {
            if (det > DELTA)
               xsector = xsector -1;  
            else if (det < -DELTA)
               ysector = ysector +1;  
            else {
               xsector = xsector -1;  
               ysector = ysector +1;  
               tmp = mapCoordinate(xsector+1, ysector, sect_size);  
               if ( tmp>=0 && tmp < sect_size*sect_size) sectors[steps++] = tmp;  
               tmp = mapCoordinate(xsector, ysector-1, sect_size);  
               if ( tmp>=0 && tmp < sect_size*sect_size) sectors[steps++] = tmp;  
            }
         }
         else if (quadrant == 2) {
            if (det > DELTA)
               ysector = ysector +1;  
            else if (det < -DELTA)
               xsector = xsector +1;  
            else {
               xsector = xsector +1;  
               ysector = ysector +1;  
               tmp = mapCoordinate(xsector-1, ysector, sect_size);  
               if ( tmp>=0 && tmp < sect_size*sect_size) sectors[steps++] = tmp;  
               tmp = mapCoordinate(xsector, ysector-1, sect_size);  
               if ( tmp>=0 && tmp < sect_size*sect_size) sectors[steps++] = tmp;  
            }
         }
         else if (quadrant == 3) {
            if (det > DELTA)
               xsector = xsector +1;  
            else if (det < -DELTA)
               ysector = ysector -1;  
            else {
               xsector = xsector +1;  
               ysector = ysector -1;  
               tmp = mapCoordinate(xsector-1, ysector, sect_size);  
               if ( tmp>=0 && tmp < sect_size*sect_size) sectors[steps++] = tmp;  
               tmp = mapCoordinate(xsector, ysector+1, sect_size);  
               if ( tmp>=0 && tmp < sect_size*sect_size) sectors[steps++] = tmp;  
            }
         }
         
         running=checkEnd(xsector, ysector, xend, yend, quadrant);   
         
         tmp = mapCoordinate(xsector, ysector, sect_size);  
         if ( tmp>=0 && tmp < sect_size*sect_size) sectors[steps++] = tmp;  
         
      }
      tmp = mapCoordinate(xsector, ysector, sect_size);  
      if ( tmp>=0 && tmp < sect_size*sect_size) sectors[steps++] = tmp;  

   }
  
   return steps;  
}


int checkEnd(int xs, int ys, int xend, int yend, int quadrant) 
{   
   switch(quadrant) {
   case 0: return (xs >= xend && ys >= yend);  break;  
   case 1: return (xs >= xend && ys <= yend);  break;  
   case 2: return (xs <= xend && ys <= yend);  break;  
   case 3: return (xs <= xend && ys >= yend);  break;  
   default: break;  
   }
   return 0;  
}

int mapCoordinate (int x, int y, int sect_size) 
{
   int tx, ty, _ty;  
   tx = x;  
   ty = y * sect_size;  
   return tx+ty;  
}


double det3Point(double x1, double y1, double x2, double y2, double x3, double y3) 
{
   return (x2-x1)*(y3-y1) - (y2-y1)*(x3-x1);  
}

void getGridPoint(int quadrant, int xsector, int ysector, double *x, double *y, int sect_size) 
{
   if (quadrant == 0) {
      *x = ((double)xsector)/((double)sect_size);  
      *y = ((double)ysector)/((double)sect_size);  
   }
   else if (quadrant == 1) {
      *x = ((double)xsector)/((double)sect_size);  
      *y = ((double)(ysector+1))/((double)sect_size);  
   }
   else if (quadrant == 2) {
      *x = ((double)(xsector+1))/((double)sect_size);  
      *y = ((double)(ysector+1))/((double)sect_size);  
   }
   else if (quadrant == 3) {
      *x = ((double)(xsector +1))/((double)sect_size);  
      *y = ((double)ysector)/((double)sect_size);  
   }
}
                    

int getQuadrant(double dx, double dy) 
{
   if (dx < 0.0) {
      if (dy < 0.0) {
         return 0;  
      } 
      else {
         return 1;  
      }
   }
   else{
      if (dy < 0.0) {
         return 3;  
      }
      else{
         return 2;  
      }
   }
}


lineContainer *LCNew(int lineNum) 
{
   lineContainer *tmp;  
   tmp = (struct lineContainer *)malloc(sizeof(struct lineContainer));  
   tmp->sectors=(struct nodeList *)malloc(sizeof(struct nodeList));  
   NLInit(tmp->sectors);  	
   tmp->lineNumber = lineNum;  
   tmp->x1 = -1;  
   tmp->y1 = -1;  
   tmp->x2 = -1;  
   tmp->y2 = -1;  
   //	printf ("LineNumber %d and LineNumber in Container %d\n", lineNum, tmp->lineNumber);  
   return tmp;  
}


nodeList *GHgetPossibleIntersects (hashSector *hash, lineContainer *line) 
{	
   listNode *ltmp, *htmp, *ret, *oldret;  
   nodeList *tmp=(nodeList *)malloc(sizeof(struct nodeList));  
   NLInit(tmp);  
   
   //	printf ("looking for sectors we (%d) are in\n", line->lineNumber);  
   ltmp = NLGetIter(line->sectors);  
   if (ltmp != NULL)
      do {
         //			printf ("In sector %d \n", (int)NLGetData(ltmp));  
         
         htmp = NLGetIter(&(hash[*(int*)NLGetData(ltmp)].lines));  
         
         if (htmp != NULL) {
            oldret = NULL;  
            ret = NLGetIter(tmp);  
            do { 			
               ret = NLGetIter(tmp);  
               
               if (ret != NULL) {					
                  do {
                     if (*(int*)(ret->data) < ((lineContainer *)htmp->data)->lineNumber) {
                        break;  
                     }
                     oldret = ret;  
                  } while ((ret = ret->next) != NULL);  
                  
                  if (ret == NULL || *(int*)(ret->data) < ((lineContainer *)htmp->data)->lineNumber) {
                     if (oldret == NULL) { 
                        NLAdd(tmp, &((lineContainer *)htmp->data)->lineNumber);  
                     }
                     else if (*(int*)(oldret->data) > ((lineContainer *)htmp->data)->lineNumber) {
                        NLInsert(tmp, oldret, &((lineContainer *)htmp->data)->lineNumber);  
                     }
                  }
               }
               else {
                  if (oldret == NULL) { 
                     NLAdd(tmp, &((lineContainer *)htmp->data)->lineNumber);  
                  }
                  else if (*(int*)(oldret->data) > ((lineContainer *)htmp->data)->lineNumber) {
                     NLInsert(tmp, oldret, &((lineContainer *)htmp->data)->lineNumber);  
                  }
               }
               ret = oldret;  
               
            } while ((htmp = htmp->next) != NULL);  
         }	
      }	while ((ltmp = ltmp->next) != NULL);  

   return tmp;  
}

void GHsortedInsert(nodeList *list, lineContainer *line) 
{
   listNode *ltmp, *oltmp;  
   
   //	printf ("Inserting at correct Position\n");  
   //	oltmp = NLGetIter(&(line->sectors));  
   
   ltmp = NLGetIter(list);  
   if (ltmp == NULL ||	((lineContainer *)ltmp->data)->lineNumber < line->lineNumber) {
      //		printf ("First Element\n");  
      NLAdd (list, line);  	
   }
   else {
      if ( ((lineContainer *)ltmp->data)->lineNumber > line->lineNumber ) {	
         oltmp = ltmp;  
         ltmp = ltmp->next;  
         //			printf ("Start iteratin\n");  
         do {
			if (ltmp == NULL || ((lineContainer *)ltmp->data)->lineNumber < line->lineNumber) {
               break;  
			}
            //			printf ("%d, ", ((lineContainer *)NLGetData(ltmp))->lineNumber);  
			oltmp = ltmp;  
         }	while ((ltmp = ltmp->next) != NULL);  
         
         NLInsert (list, oltmp, line);  	
	  }
      
   }  
}


void GHsetLine(hashSector **hsecs, int *numDirty, hashSector *hash,  nodeList *dirtySectors, 
               lineContainer *line, int index, t_point p1, t_point p2, int point1, int point2, 
               int sect_size) 
{
   int sects, i;   
   int *tmp;  
   listNode *ltmp, *htmp, *ret, *oldret;  
   struct lineElement *lE;  
   
   if (p1.x > -1) {	
      ltmp = NLGetIter(line->sectors);  
      
      line->x1 = p1.x;  
      line->y1 = p1.y;  
      line->x2 = p2.x;  
      line->y2 = p2.y;  
      
      sects = lineSects(p1.x, p1.y, p2.x, p2.y, sectors, sect_size);      
      
      for (i=0;  i<sects;  i++) {
         lE = lineElement_new (point1, point2, line);  
         ret = NLAdd(&(hash[sectors[i]].lines), lE);  
         ltmp = NLGetIter(&hash[sectors[i]].lines);  	
         
         if (NLsizeOf(&(hash[sectors[i]].lines)) > 1 && !hash[sectors[i]].dirty) {
            hash[sectors[i]].dirty = 1;  // Mark the Sector as dirty
            hsecs[*numDirty]=&(hash[sectors[i]]);  
            *numDirty = *numDirty + 1;  
         }
      }
   }
}


struct lineElement *lineElement_new(int p1, int p2, struct lineContainer *lc) 
{
   struct lineElement *el;  
   if (lineElement_freelist != NULL) {
      el = lineElement_freelist;  
      lineElement_freelist = lineElement_freelist->nextHook;  
   } 
   else {
      el = (struct lineElement*)malloc(sizeof(struct lineElement));  
      maxlineElements_inUse ++;  
   }
   el->p1 = p1;  
   el->p2 = p2;  
   el->lc = lc;  
   el->nextHook = NULL;  
   maxUsed++;  

   return el;  
}


void lineElement_del(struct lineElement *el) 
{
   el->nextHook = lineElement_freelist;  
   lineElement_freelist = el;  
}


void lineElement_free(struct lineElement *el) 
{
   if (el!=NULL) {
      free(el);  
   }
}


struct lineElement *getLineElement_FreeList() 
{
   return lineElement_freelist;  
}


void resetLineElement_FreeList() 
{
	lineElement_freelist = NULL;  
}


int maxInUse() 
{
	return maxlineElements_inUse;  
}


int maxUsedEL() 
{
	return maxUsed;  
}


int allocSectorArray(int sect_size) 
{
   //3mal weil wir wenn wir über die Ecke gehen auch die Diagonalen mitnehmen
   sectors = (int *)malloc(6*sect_size*sizeof(int));  
}


void freeSectorArray() 
{
	free(sectors);  
}



/* END_INSERT */
