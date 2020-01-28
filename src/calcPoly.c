/*****************************************************************************/
/*                                                                           */
/*                       Random Polygon Generator (RPG)                      */
/*                                                                           */
/* Copyright (C)  T. Auer, M. Gschwandtner, M. Heimlich, M. Held   1994-2020 */
/*                                                                           */
/*****************************************************************************/

#include "calcPoly.h"
#include "stateLine.h"
#include "pointList.h"
#include "pointOps.h"
#include "polyOps.h"
#include "MCPolygon.h"
//#include "formOps.h"

#include <stdlib.h>

/********************************************************************/
/*                                                                  */
/* Constants and the like                                           */
/*                                                                  */
/********************************************************************/

static enum t_calcType CPstate = CP_IDLE;  
static int CPparameter = RPG_UNDEFINED;  
static int MCP = FALSE;  

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
void CPsetState(enum t_calcType state, int parameter)
{
  CPstate = state;  
  CPparameter = parameter;  

  switch(CPstate)
    {
    case CP_IDLE:
      SLsetState(ST_OK);  
      break;  

    case CP_POINTS:
      SLsetState(ST_POINTS);  
      break;  

    case CP_CIRCLEPOINTS:
      SLsetState(ST_CIRCLEPOINTS);  
      break;  

    case CP_SMOOTH:
      SLsetState(ST_SMOOTH);  
      break;  

    case CP_BOUNCE:
      SLsetState(ST_BOUNCE);  
      break;  

    case CP_XMONO:
      SLsetState(ST_XMONO);  
      break;  

    case CP_ARRANGE_STAR:
      SLsetState(ST_STAR);  
      break;  

    case CP_FAST_STAR: 
      SLsetState(ST_STAR);  
      break;  

    case CP_STAR:
      SLsetState(ST_STAR);  
      break;  

    case CP_GROWING:
      SLsetState(ST_GROW);  
      break;  

    case CP_GROWING_II:
      SLsetState(ST_GROW_II);  
      break;  

    case CP_2OPT:
      SLsetState(ST_2OPT);  
      break;  

    case CP_2OPT_II:
      SLsetState(ST_2OPT_II);  
      break;  

    case CP_2OPT_III:
      SLsetState(ST_2OPT_III);  
      break;  

    case CP_SPACE_PART:
      SLsetState(ST_SPACE_PART);  
      break;  

    case CP_REJECT:
      SLsetState(ST_REJECT);  
      break;  

    case CP_SEARCH:
      SLsetState(ST_SEARCH);  
      break;  

    case CP_TRIANGLE:
      SLsetState(ST_TRIANGLE);  
      break;  

    default:
      break;  
    }
}

void CPdoCalc()
{
   t_pointList *pointList;  
   
   if (CPstate != CP_IDLE)  {
      pointList = POgetPointList();  
      
      switch (CPstate)   {
      case CP_SMOOTH: /* smooth the polygon */
         YOsmoothPoly(pointList);  
         break;  
         
      case CP_POINTS: /* compute a given number of random points */
         if (CPparameter !=  RPG_UNDEFINED)
            POgenerateRandomPoints(CPparameter);  
         break;  
         
      case CP_CIRCLEPOINTS: /* compute a given number of random points 
                               within  the unit circle*/
         if (CPparameter != RPG_UNDEFINED)
            POgenerateRandomCirclePoints(CPparameter);  
         break;  
         
      case CP_CLUSTERPOINTS: /* compute random clustered points */
         if (CPparameter != RPG_UNDEFINED)
            POgenerateClusterPoints(CPparameter);  
         break;  
         
      case CP_BOUNCE: /* Bouncing polygons, algorithm by O'Rourke */
         if (CPparameter != RPG_UNDEFINED)
            YObouncePoly(CPparameter, pointList);  
         break;  
         
      case CP_XMONO: /* xmonotone Polygons */
         YOxmonoPoly(pointList);  
         break;  
         
      case CP_ARRANGE_STAR: /* star shaped Polygons */
         YOstarPoly(pointList);  
         break;  
         
      case CP_FAST_STAR: /* fast star-shaped polygon, i.e. locate a point
                            within the CH and sort the points around */
         YOfastStarPoly(pointList);  
         break;  
         
      case CP_STAR: /* new star-shaped polygons: get kernels
                       without arrangement */
         YOnewStarPoly(pointList);  
         break;  
         
      case CP_GROWING: /* growing polygon: add points in a way that
                          no point lies within the CH */
         YOgrowingPoly(pointList);  
         break;  
         
      case CP_GROWING_II: /* growing polygonII: add points in a way that
                             no point lies within the CH */
         YOgrowingPolyII(pointList);  
         break;  
         
      case CP_2OPT: /* 2-opt polygon: generate a random polygon, 
                       apply 2-opt moves to make it simple */
         YO2optPoly(pointList);  
         break;  
         
      case CP_2OPT_II: /* 2-opt polygon II: generate a random polygon, 
                          apply 2-opt moves to make it simple */
         YO2optPolyII(pointList);  
         break;  
         
      case CP_2OPT_III: /* 2-opt polygon III: generate a random polygon, 
                           apply 2-opt moves to make it simple */
         YO2optPolyIII(pointList);  
         break;  
         
      case CP_SPACE_PART: /* create polygon by recursive space 
                             partitioning */
         YOspPoly(pointList);  
         break;  
         
      case CP_REJECT: /* create polygon by generating a random 
                         polygon, if it's not simple, reject it! */
         YOrejectPoly(pointList);  
         break;  
         
      case CP_SEARCH: /* create polygon by searching the tree and
                         avoiding dead subtrees */
         YOsearchPoly(pointList);  
         break;  
         
      case CP_TRIANGLE: /* create polygon by searching the tree and
                           avoiding dead subtrees */
         YOtriangPoly(CPparameter, pointList);  
         break;  
         
      case CP_HOLES: /* create multiply-connected polygon */
         if (CPparameter != RPG_UNDEFINED)
            YOholesPoly(CPparameter, pointList);  
         break;  
         
      default:
         break;  
      }
	
      /* set the state to idle, we want to calculate only 
         once! */
      CPsetState(CP_IDLE, RPG_UNDEFINED);  
   }

   return;  
}


