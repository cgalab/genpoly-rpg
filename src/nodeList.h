/*****************************************************************************/
/*                                                                           */
/*                       Random Polygon Generator (RPG)                      */
/*                                                                           */
/* Copyright (C)  T. Auer, M. Gschwandtner, M. Heimlich, M. Held   1994-2020 */
/*                                                                           */
/*****************************************************************************/

#ifndef INC_NODE_LIST
#define INC_NODE_LIST
#include <stdio.h>

typedef struct  listNode {
		struct listNode *prev, *next;  
		void *data;  	
}listNode;  

typedef struct nodeList{
		struct listNode *start;  	
	    int size;  //Warnung INT könnte zu klein werden
}nodeList;  


void NLInit(nodeList *list);  
void NLAdd(nodeList *list, void *data);  
void NLDel(nodeList *list, void *data);  
void *NLGet(nodeList *list, int index);  
listNode *NLInsert(nodeList *list, listNode *node, void *data);  
void NLClear(nodeList *list);  
struct listNode *NLGetIter(nodeList *list);  
struct listNode *NLGetNext(struct listNode *node);  
int NLsizeOf(nodeList *list);  
void *NLGetData(listNode *node);  
void NLDelByListNode(nodeList *list, listNode *node);  
void NLIncFree(int inc);  
void NLDecFree(int dec);  
int NLGetFree();  
int NLGetMaxNodes();  
//NLfree(nodeList *list);  
void NLFreeNode(listNode *n);  
struct listNode *NLget_FreeList();  
void NLreset_FreeList();  	

#endif
