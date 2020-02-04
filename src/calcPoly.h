/*****************************************************************************/
/*                                                                           */
/*                       Random Polygon Generator (RPG)                      */
/*                                                                           */
/* Copyright (C)  T. Auer, M. Gschwandtner, M. Heimlich, M. Held   1994-2020 */
/*                                                                           */
/*****************************************************************************/

#ifndef __CALCPOLY_H_
#define __CALCPOLY_H_

/********************************************************************/
/*                                                                  */
/* Constants and the like                                           */
/*                                                                  */
/********************************************************************/

#define FOREACH_CALCTYPE(CALCTYPE) \
        CALCTYPE(CP_IDLE)          \
        CALCTYPE(CP_POINTS)        \
        CALCTYPE(CP_CIRCLEPOINTS)  \
        CALCTYPE(CP_CLUSTERPOINTS) \
        CALCTYPE(CP_SMOOTH)        \
        CALCTYPE(CP_BOUNCE)        \
        CALCTYPE(CP_XMONO)         \
        CALCTYPE(CP_ARRANGE_STAR)  \
        CALCTYPE(CP_FAST_STAR)     \
        CALCTYPE(CP_STAR)          \
        CALCTYPE(CP_GROWING)       \
        CALCTYPE(CP_GROWING_II)    \
        CALCTYPE(CP_2OPT)          \
        CALCTYPE(CP_2OPT_II)       \
        CALCTYPE(CP_2OPT_III)      \
        CALCTYPE(CP_SPACE_PART)    \
        CALCTYPE(CP_REJECT)        \
        CALCTYPE(CP_SEARCH)        \
        CALCTYPE(CP_TRIANGLE)      \
        CALCTYPE(CP_HOLES)

#define GENERATE_ENUM(ENUM) ENUM,
#define GENERATE_STRING(STRING) #STRING,

enum t_calcType {
  FOREACH_CALCTYPE(GENERATE_ENUM)
};


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
void CPsetState(enum t_calcType state, int parameter);  

void CPdoCalc();  

#endif
