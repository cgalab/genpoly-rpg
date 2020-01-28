/*****************************************************************************/
/*                                                                           */
/*                       Random Polygon Generator (RPG)                      */
/*                                                                           */
/* Copyright (C)  T. Auer, M. Gschwandtner, M. Heimlich, M. Held   1994-2020 */
/*                                                                           */
/*****************************************************************************/

#include <stdlib.h>
#include "nodeList.h"

static struct listNode *freenodes = NULL;  // Hier werden nicht mehr benötigte 
static int free_Nodes = 0;  		 // Listenelement gespeichert
static int maxNodes = 0;  

void NLInit(nodeList *list) {
//	list 	    = (struct nodeList*)malloc(sizeof(struct nodeList));  
	list->start = NULL;  
	list->size  = 0;  
}

listNode *NLAdd(nodeList *list, void *data) {
	struct listNode *tmp;  

	tmp               = list->start;  

	if (freenodes == NULL)	{
		list->start       = (struct listNode*)malloc(sizeof(struct listNode));  
        maxNodes++;  
		//		printf ("New node (Free Nodes: %i)\n", NLGetFree());  
	} else {
//		printf ("Use existing Node(Free Nodes: %i)\n", NLGetFree());  
		list->start = freenodes;  
		freenodes = freenodes->next;  
		NLDecFree(1);  
	}
	
	list->start->next = tmp;  
	if (tmp != NULL) tmp->prev=list->start;  
	list->start->prev = NULL;  
	list->start->data = data;  
	list->size ++;  
	return list->start;  
}

void NLDel(nodeList *list, void *data) {
	struct listNode *tmp, *otmp, *ftmp;  

	otmp = NULL;  
	tmp               = list->start;  
//	printf ("Del Node\n");  

	if (tmp != NULL) {
		do {
			if (tmp->data == data) {
				if (otmp == NULL) {
					list->start=list->start->next;  
					tmp->next = freenodes;  
					freenodes = tmp;  
					if (list->start != NULL)list->start->prev = NULL;  
					
				} else {
					ftmp = otmp->next;  
					otmp->next = tmp->next;  
					otmp->next->prev = otmp;  
					tmp->next = freenodes;  
					freenodes = tmp;  
				}
				break;  
			}
			otmp = tmp;  
		}while ((tmp= tmp->next) != NULL);  		
	NLIncFree(1);  
	}
	list->size--;  
}



void *NLGet(nodeList *list, int index) {
	struct listNode *tmp;  
	int i;  
	
	tmp = list->start;  
	for (i=0;  i<index;  i++) {
		if (tmp != NULL)
			tmp = tmp->next;  
		else break;  
	}
	if (tmp != NULL)
		return tmp->data;  
}

void *NLGetData(listNode *node) {
	if (node != NULL) return node->data;  
	return NULL;  		
}


void NLClear(nodeList *list) {
	
	struct listNode *tmp, *ftmp;  

	tmp               = list->start;  
	if (tmp != NULL)
//	printf ("Clear Nodes\n");  
		do {
			ftmp = tmp->next;  
			tmp->next=freenodes;  
			freenodes = tmp;  
			NLIncFree(1);  
		}while ((tmp= ftmp) != NULL);  		
	list->start = NULL;  
	list->size = 0;  
	
}

int NLsizeOf(nodeList *list) {
/*	int size;  	
	struct listNode *tmp;  
	
	tmp = list.start;  
	while (tmp != NULL) {
		size++;  
		tmp = tmp->next;  
	}*/

	return list->size;  	
	
}


void NLFreeNode(listNode *n) {
	if (n!=NULL) {
			free(n);  
	}
}

struct listNode *NLGetIter(nodeList *list) {
     if (list != NULL)
		return list->start;  
	else 
		return NULL;  
}

struct listNode *NLGetNext(struct listNode *node) {
	if (node != NULL)
		if (node->next != NULL)
			return node->next;  
	return NULL;  

}

listNode *NLInsert(nodeList *list, listNode *node, void *data) {
	
	struct listNode *tmp;  

	if (freenodes == NULL) {	
//		printf ("Insert Node\n");  
		maxNodes++;  
		tmp = (struct listNode*)malloc(sizeof(struct listNode));  
	} else {
//		printf ("Insert existing Node\n");  
		tmp = freenodes;  
		freenodes = freenodes->next;  
		NLDecFree(1);  
	}


	tmp->next = NULL;  
	tmp->next = node->next;  
	if (node->next != NULL) node->next->prev = tmp;  
	node->next = tmp;  
	tmp->prev = node;  
	tmp->data = data;  
	list->size ++;  
	return tmp;  

}


void NLDelByListNode(nodeList *list, listNode *node) {

	//printf ("Del by listnode\n");  	
	if (list->start != NULL) {
/*		printf ("list: %p\n", list);  
		printf ("list->start: %p\n", list->start);  
		printf ("list->start->prev: %p\n", list->start->prev);  
*/
		if (node->prev == NULL) {
			list->start = node->next;  		
		    if (list->start != NULL) list->start->prev = NULL;  

			}
		else {
			node->prev->next = node->next;  
			if (node->next != NULL)	node->next->prev = node->prev;  
		}
			node->next = freenodes;  
			freenodes = node;  
			NLIncFree(1);  
		list->size--;  
	}
}


void NLIncFree(int inc) {
	free_Nodes += inc;  
}
void NLDecFree(int dec) {
	free_Nodes -= dec;  
}

int NLGetFree() {
	return free_Nodes;  
}

int NLGetMaxNodes() {
	return maxNodes;  
}
struct listNode *NLget_FreeList() {
	return freenodes;  
}
void NLreset_FreeList() {
	freenodes=NULL;  
}
