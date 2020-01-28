/*****************************************************************************/
/*                                                                           */
/*                       Random Polygon Generator (RPG)                      */
/*                                                                           */
/* Copyright (C)  T. Auer, M. Gschwandtner, M. Heimlich, M. Held   1994-2020 */
/*                                                                           */
/*****************************************************************************/

#include "quadEdge.h"
#include "eralloc.h"

t_lineInfo QEdefaultLine = {-1, -1};  



t_edgeInfo *QEnewEdge()
{
  t_quadEdge *qEdge;  
  t_edgeInfo *edge;  
  int count;  
  
  qEdge = (t_quadEdge*) eralloc(sizeof(t_quadEdge));  

  edge = qEdge[0];  

  for (count=0;  count<4;  count++)
    {
      edge->rot = count;  
      edge->nrOfLines = 0;  
      edge->lineInfo = NULL;  
      edge->next = NULL;  
      edge->face = NULL;  
      edge->visited = 0;  /* clear mark for deletion */
      edge++;  
    }
    
  edge = qEdge[0];  

  return(edge);     
}

void QEfreeEdge(t_edgeInfo *edge)
{
  t_quadEdge *qEdge;  
  t_edgeInfo *curEdge;  
  int count;  

  qEdge = QEbaseAddr(edge);  
  curEdge = (t_edgeInfo *) qEdge;  

  for (count=0;  count<4;  count++)
    {
      if (curEdge->nrOfLines > 0)
	erfree(edge->lineInfo);  
      curEdge++;  
    }
    
  erfree(qEdge);  
}


/* QEaddr:
 *      Given the address of the edge e, and an rot offset, calculates
 *      the new effective address, where the new address is limited to, 
 *      the boundaries of our current quad-edge structure.
 */
t_edgeInfo *QEaddr(t_edgeInfo *edge, int offset)
{
  t_edgeInfo *edgeAddr;  
  int index;  
 
  index = (((edge->rot+offset)%4)-edge->rot);  
  edgeAddr = edge+index;  

  return(edgeAddr);  
}

/* QEbaseAddr:
   given the address of the edge e, calculate the quad-edge structure 
   that e belongs to. */
t_quadEdge *QEbaseAddr(t_edgeInfo *edge)
{
  t_quadEdge *qEdge;  
 
  qEdge = (t_quadEdge *)(edge - edge->rot);  
  
  return(qEdge);  
}


/* QErot:
 *      Edge function Rot:   (e, r).Rot  = (e, r+1)
 */
t_edgeInfo *QErotEdge(t_edgeInfo *edge)
{
  return(QEaddr(edge, 1));  
}


/* QEsymEdge:
 *      Edge function Sym:   (e, r).Sym  = (e, r+2)
 */
t_edgeInfo *QEsymEdge(t_edgeInfo *edge)
{
  return(QEaddr(edge, 2));  
}

/* QEirot:
 *      Edge function IRot:  (e, r).IRot = (e, r+3)
 */
t_edgeInfo *QEirotEdge(t_edgeInfo *edge)
{
  return(QEaddr(edge, 3));  
}


/* QEonextEdge:
 *      Edge function Onext: (e, r).Onext = e[r].Next
 */
t_edgeInfo *QEonextEdge(t_edgeInfo *edge)
{
  return(edge->next);  
}

/* QEright: same as QErotEdge */
t_edgeInfo *QEright(t_edgeInfo *edge)
{
  return(QErotEdge(edge));  
}

/* QEleft: same as QEirotEdge */
t_edgeInfo *QEleft(t_edgeInfo *edge)
{
  return(QEirotEdge(edge));  
}


/* QElnext: e.Lnext = e.IRot.Onext.Rot */ 
t_edgeInfo *QElnextEdge(t_edgeInfo *edge)
{
  return(QErotEdge(QEonextEdge(QEirotEdge(edge))));  
}



t_face *QElFace(t_edgeInfo *edge)
{
  t_edgeInfo *auxEdge;  

  auxEdge = QEirotEdge(edge);  
  return(auxEdge->face);  
}

void QEsetLFace(t_edgeInfo *edge, t_face *face)
{
  t_edgeInfo *auxEdge;  

  auxEdge = QEirotEdge(edge);  
  auxEdge->face = face;  
}

t_face *QErFace(t_edgeInfo *edge)
{
  t_edgeInfo *auxEdge;  

  auxEdge = QErotEdge(edge);  
  return(auxEdge->face);  
}

void QEsetRFace(t_edgeInfo *edge, t_face *face)
{
  t_edgeInfo *auxEdge;  

  auxEdge = QErotEdge(edge);  
  auxEdge->face = face;  
}

t_point QEorig(t_edgeInfo *edge)
{
  return(edge->point);  
}

void QEsetOrig(t_edgeInfo *edge, t_point *point)
{
  edge->point = *point;  
}

t_point QEdest(t_edgeInfo *edge)
{
  return(QEorig(QEsymEdge(edge)));  
}

void QEsetDest(t_edgeInfo *edge, t_point *point)
{
  QEsetOrig(QEsymEdge(edge), point);  
}

void QEaddLineInfo(t_edgeInfo *edge, t_lineInfo lineInfo)
{
  if (!QEcheckLineInfo(edge, lineInfo))
    {
      edge->nrOfLines++;  

      edge->lineInfo = (t_lineInfo *) errealloc(edge->lineInfo, 
						edge->nrOfLines*
						sizeof(t_lineInfo));  
      *((edge->lineInfo)+edge->nrOfLines-1) = lineInfo;  
      
      /* symmetric edge lies on the same line */
      QEaddLineInfo(QEsymEdge(edge), lineInfo);  
    }
}

int QEcheckLineInfo(t_edgeInfo *edge, t_lineInfo lineInfo)
{
  int found=FALSE;  
  int count;  

  for (count=0;  count<edge->nrOfLines;  count++)
    {
      found = found ||
	((((edge->lineInfo)+count)->pIndex1 == lineInfo.pIndex1) &&
	 (((edge->lineInfo)+count)->pIndex2 == lineInfo.pIndex2));  
    }
  
  return(found);  
}


void QElineInfoInit(t_lineInfo *lineInfo, int point1, int point2)
{
  lineInfo->pIndex1 = MIN(point1, point2);  
  lineInfo->pIndex2 = MAX(point1, point2);  
}

int QEgetNrOfLines(t_edgeInfo *edge)
{
  return(edge->nrOfLines);  
}

t_lineInfo QEgetLineInfo(t_edgeInfo *edge, int index)
{
  if ((index > 0) && (index <= edge->nrOfLines))
    return(*((edge->lineInfo)+index-1));  
  else
    return(QEdefaultLine);  
}

int QElineInfoP1(t_lineInfo *lineInfo)
{
  return(lineInfo->pIndex1);  
}

int QElineInfoP2(t_lineInfo *lineInfo)
{
  return(lineInfo->pIndex2);  
}


