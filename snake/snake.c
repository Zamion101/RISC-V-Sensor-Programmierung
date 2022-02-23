#include <stdint.h>
#include <stdlib.h>
#include "snake.h"


void initPositionArray(PositionArray *array){
  array->head = NULL;
  array->tail = NULL;
  array->size = 0;
}

PositionNode *createNode(Position position){
  PositionNode * pNode;
  pNode = (PositionNode *)malloc(sizeof(PositionNode));
  pNode->x = position.x;
  pNode->y = position.y;
  pNode->prev = NULL;
  pNode->next = NULL;
  return pNode;
}

void addAtTail(PositionArray *array, Position position){
  PositionNode *node;
  node = createNode(position);

  //if list is empty.
	if(array->head == NULL)
	{
		array->head = node;
		array->tail = node;
	}
	else
	{
    node->prev = array->tail;
		array->tail->next  = node;
		array->tail = array->tail->next;
	}
  array->size += 1;
}

//Delete item from the end of list.
void delFromTail(PositionArray *array)
{
	PositionNode *temp;

	Position item;


	if(array->tail == NULL)
	{	
		printf("\nList is Empty ..."); 	
		return;
	}
	else
	{
		temp = array->head;

		while((temp->next > 0) && (temp->next != array->tail))
		{ temp = temp->next;}

    // item = (Position)malloc(sizeof(Position));
    // item.x = array->tail->x;
    // item.y = array->tail->y;
		
    array->tail = temp; 
		array->tail->next = NULL;		
	}
  array->size -= 1;

	return;
}

//Add new position at begning of the list.
void addAtHead(PositionArray *array,Position position)
{
	PositionNode * node;
	node = createNode(position);

	//if list is empty.
	if(array->head == NULL)
	{
		array->head = node;
		array->tail = node;
	}
	else
	{
		node->next 	= array->head;
    array->head->prev = node;
		array->head = node;
	}		
  array->size += 1;
}

//Delete item from Start of list.
void delFromHead(PositionArray *array)
{
	Position item;

	if(array->head == NULL)
	{	
		printf("\nList is Empty ..."); 	
		return;
	}
	else
	{
    // item = (Position)malloc(sizeof(Position));
    // item.x = array->head->x;
    // item.y = array->head->y;

		array->head = array->head->next;
		array->head->prev = NULL;		
	}
  array->size -= 1;

	return;
}