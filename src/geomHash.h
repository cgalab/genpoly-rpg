/*****************************************************************************/
/*                                                                           */
/*                       Random Polygon Generator (RPG)                      */
/*                                                                           */
/* Copyright (C)  T. Auer, M. Gschwandtner, M. Heimlich, M. Held   1994-2020 */
/*                                                                           */
/*****************************************************************************/

#include "nodeList.h"
#include "basicGeom.h"
#include "voidIncArray.h"




#ifndef INC_GEOM_HASH
#define INC_GEOM_HASH

#define EPSILON_GEOM 0.00001

typedef struct lineContainer {
	
	nodeList *sectors;  
	double x1, y1, x2, y2;  //This is redundant but needed for deleting 
                            //the line from its Sectors
	int lineNumber;  
	
} lineContainer;  

typedef struct hashSector {
	int dirty, sectorNumber;  
	nodeList lines;  	
} hashSector;  

typedef struct lineElement {
	int p1, p2;  
	struct lineElement *nextHook;  //this is only for memory management purposes
	struct lineContainer *lc;  
} lineElement;  


int lineSects(double x1, double y1, double x2, double y2, int *sectors, int sect_size);  
double det3Point(double x1, double y1, double x2, double y2, double x3, double y3);  
void getGridPoint(int quadrant, int xsector, int ysector, double *x, double *y, int sect_size);  
int getQuadrant(double dx, double dy);  
int mapCoordinate (int x, int y, int sect_size);  
int checkEnd(int xs, int ys, int xend, int yend, int quadrant);  

void GHsetLine(hashSector **hsecs, int *numDirty, hashSector *hash, nodeList *dirtySectors, lineContainer *line, int index, t_point p1, t_point p2, int point1, int point2, int sect_size);  
lineContainer *LCNew(int lineNum);  
nodeList *GHgetPossibleIntersects (hashSector *hash, lineContainer *line);  

struct lineElement  *lineElement_new(int p1, int p2, struct lineContainer *lc);  
void lineElement_del(struct lineElement *el);  
void lineElement_free(struct lineElement *el);  
int maxInUse();  
int maxUsedEL();  
struct lineElement *getLineElement_FreeList();  
void resetLineElement_FreeList();  
int allocSectorArray(int sect_size);  
void freeSectorArray();  



/* END_INSERT */
#endif
