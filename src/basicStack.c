/*****************************************************************************/
/*                                                                           */
/*                       Random Polygon Generator (RPG)                      */
/*                                                                           */
/* Copyright (C)  T. Auer, M. Gschwandtner, M. Heimlich, M. Held   1994-2020 */
/*                                                                           */
/*****************************************************************************/

#include "basicStack.h"
#include "basicDefs.h"
#include "eralloc.h"

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
void BSinitStack(t_stack *aStack)
{
  aStack->top = NULL;  
}


void BSpushElem(t_stack *aStack, caddr_t elem)
{
  struct t_stackElem *newElem;  

  newElem = (struct t_stackElem *) eralloc(sizeof(struct t_stackElem));  

  newElem->elem = elem;  
  newElem->next = aStack->top;  

  aStack->top = newElem;  
}


caddr_t BSviewTop(t_stack *aStack)
{
  return(aStack->top->elem);  
}


caddr_t BSpopElem(t_stack *aStack)
{
  struct t_stackElem *oldTop;  
  caddr_t elem;  

  oldTop = aStack->top;  
  elem = oldTop->elem;  
  aStack->top = oldTop->next;  

  erfree(oldTop);  

  return(elem);  
}


int BSisEmpty(t_stack *aStack)
{
  return((aStack->top) == NULL);  
}

void BSfreeIt(struct t_stackElem *elem)
{
  if (elem != NULL)
    {
      BSfreeIt(elem->next);  
      erfree(elem);  
    }
}

void BSfreeStack(t_stack *aStack)
{
  BSfreeIt(aStack->top);  
}
  


