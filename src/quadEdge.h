/*****************************************************************************/
/*                                                                           */
/*                       Random Polygon Generator (RPG)                      */
/*                                                                           */
/* Copyright (C)  T. Auer, M. Gschwandtner, M. Heimlich, M. Held   1994-2020 */
/*                                                                           */
/*****************************************************************************/

#include "basicGeom.h"
#include "basicLine.h"

#ifndef __QUADEDGE_H_
#define __QUADEDGE_H_

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
typedef struct {
  int visited;  
} t_face;  

typedef struct {
  int pIndex1, pIndex2;  
} t_lineInfo;  

extern t_lineInfo QEdefaultLine;  

typedef struct t_edgeStruct t_edgeInfo;  

struct t_edgeStruct {
  t_edgeInfo *next;  
  int rot;  
  t_lineInfo *lineInfo;  
  int nrOfLines;  
  int visited;  /* marker for deletion */
  t_point point;  
  t_face  *face;  
};  

typedef t_edgeInfo t_quadEdge[4];  

/********************************************************************/
/*                                                                  */
/* Macros                                                           */
/*                                                                  */
/********************************************************************/

/********************************************************************/
/*                                                                  */
/* Procedures and functions                                         */
/*                                                                  */
/********************************************************************/
t_edgeInfo *QEnewEdge();  

void QEfreeEdge(t_edgeInfo *edge);  

/* QEaddr:
 *      Given the address of the edge e, and an rot offset, calculates
 *      the new effective address, where the new address is limited to, 
 *      the boundaries of our current quad-edge structure.
 */
t_edgeInfo *QEaddr(t_edgeInfo *edge, int offset);  

/* QEbaseAddr:
   given the address of the edge e, calculate the quad-edge structure 
   that e belongs to. */
t_quadEdge *QEbaseAddr(t_edgeInfo *edge);  

/* QErot:
 *      Edge function Rot:   (e, r).Rot  = (e, r+1)
 */
t_edgeInfo *QErotEdge(t_edgeInfo *edge);  


/* QEsymEdge:
 *      Edge function Sym:   (e, r).Sym  = (e, r+2)
 */
t_edgeInfo *QEsymEdge(t_edgeInfo *edge);  

/* QEirot:
 *      Edge function IRot:  (e, r).IRot = (e, r+3)
 */
t_edgeInfo *QEirotEdge(t_edgeInfo *edge);  


/* QEonextEdge:
 *      Edge function Onext: (e, r).Onext = e[r].Next
 */
t_edgeInfo *QEonextEdge(t_edgeInfo *edge);  

/* QEright: same as QErotEdge */
t_edgeInfo *QEright(t_edgeInfo *edge);  

/* QEleft: same as QEirotEdge */
t_edgeInfo *QEleft(t_edgeInfo *edge);  


/* QElnext: e.Lnext = e.IRot.Onext.Rot */ 
t_edgeInfo *QElnextEdge(t_edgeInfo *edge);  

t_face *QElFace(t_edgeInfo *edge);  

void QEsetLFace(t_edgeInfo *edge, t_face *face);  

t_face *QErFace(t_edgeInfo *edge);  

void QEsetRFace(t_edgeInfo *edge, t_face *face);  

t_point QEorig(t_edgeInfo *edge);  

void QEsetOrig(t_edgeInfo *edge, t_point *point);  

t_point QEdest(t_edgeInfo *edge);  

void QEsetDest(t_edgeInfo *edge, t_point *point);  

void QEaddLineInfo(t_edgeInfo *edge, t_lineInfo lineInfo);  

int QEcheckLineInfo(t_edgeInfo *edge, t_lineInfo lineInfo);  

void QElineInfoInit(t_lineInfo *lineInfo, int point1, int point2);  

int QEgetNrOfLines(t_edgeInfo *edge);  

t_lineInfo QEgetLineInfo(t_edgeInfo *edge, int index);  

int QElineInfoP1(t_lineInfo *lineInfo);  

int QElineInfoP2(t_lineInfo *lineInfo);  

#endif
