//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2020.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Software Update
 Module         LinkedList 
 Description    Deklaration of Runtime Lists with Heap usage

 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------

#ifndef LINKED_LIST_H
#define LINKED_LIST_H

//#include "..\Interface\SU_Interface.h"

typedef struct list_node
{
	T_SUP_MANIFEST_RECORD record;
	struct list_node * next;
}list_node;

typedef struct update_list{
	struct list_node * head;
}update_list;


list_node* LinkedList_CreateNode(T_SUP_MANIFEST_RECORD record);
update_list* LinkedList_Make(void);
void LinkedList_Append(T_SUP_MANIFEST_RECORD record, update_list* list);
void LinkedList_Delete(update_list* list);


#endif
