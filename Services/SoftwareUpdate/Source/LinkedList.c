//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2020.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Software Update
 Module         LinkedList 
 Description    Implementation of Runtime Lists with Heap usage

 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------

#include "System/Interface/common_type.h"
#include "Services/SoftwareUpdate/Interface/SU_Interface.h"
#include "RTOS/Interface/rtos.h"
#include "LinkedList.h"

#define LM_ALLOCATE     pvPortMallocSlow
#define LM_FREE         vPortFreeSlow
// or
//#define LM_ALLOCATE     pvPortMalloc
//#define LM_FREE         vPortFree
        

//--------------------------------------------------------------------------------------------------
/*!
 \brief     Creates new record in linked list
 \author    Piotr Kmiecinski
 \date      2020-02-24
 \param     record entry to create
 \return    pointer to new node
 \warning   none
 \test
 \n test-date:           
 \n by:                  Piotr Kmiecinski
 \n environment:         End-to-end
 \n intention:           
 \n result module test:  OK
 \n result Lint Level 3:  
*/
//--------------------------------------------------------------------------------------------------
list_node * LinkedList_CreateNode(T_SUP_MANIFEST_RECORD record) {
	list_node * newNode = (list_node*)LM_ALLOCATE(sizeof(list_node));
	if (!newNode) {
		return NULL;
	}
	newNode->record = record;
	newNode->next = NULL;
	return newNode;
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief     Creates a head of list
 \author    Piotr Kmiecinski
 \date      2020-02-24
 \return    pointer to new node
 \warning   none
 \test
 \n test-date:           
 \n by:                  Piotr Kmiecinski
 \n environment:         End-to-end
 \n intention:           
 \n result module test:  OK
 \n result Lint Level 3:  
*/
//--------------------------------------------------------------------------------------------------
update_list* LinkedList_Make(void) {
	update_list * list = (update_list*)LM_ALLOCATE(sizeof(update_list)); 
    if (!list) {
		return NULL;
    }
	list->head = NULL;
	return list;
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief     Add new entry to list
 \author    Piotr Kmiecinski
 \date      2020-02-24
 \param     record manifest entry structure
 \param 	list pointer to list
 \warning   none
 \test
 \n test-date:           
 \n by:                  Piotr Kmiecinski
 \n environment:         End-to-end
 \n intention:           
 \n result module test:  OK
 \n result Lint Level 3:  
*/
//--------------------------------------------------------------------------------------------------
void LinkedList_Append(T_SUP_MANIFEST_RECORD record, update_list * list) {
	list_node * current = NULL;
	if (list->head == NULL) {
		list->head = LinkedList_CreateNode(record);
	}
	else {
		current = list->head;
		while (current->next != NULL) {
			current = current->next;
		}
		current->next = LinkedList_CreateNode(record);
	}
}

//--------------------------------------------------------------------------------------------------
/*!
 \brief     Deletes list
 \author    Piotr Kmiecinski, Klaus Pose
 \date      2020-02-24,       2020-07-27
 \param 	list pointer to list
 \warning   none
 \test
 \n test-date:           
 \n by:                  Piotr Kmiecinski
 \n environment:         End-to-end
 \n intention:           
 \n result module test:  OK
 \n result Lint Level 3:  
*/
//--------------------------------------------------------------------------------------------------
void LinkedList_Delete(update_list * list) {
	list_node * current = list->head;
	list_node * next = current;
	while (current != NULL) {
		next = current->next;
		LM_FREE(current);
		current = next;
	}
	LM_FREE(list);
}
