#include <stdint.h>

#ifndef __snake
    #define __snake
    typedef struct Position{
        int8_t x;
        int8_t y;
    } Position;


    typedef enum{
        UP,
        DOWN,
        LEFT,
        RIGHT,
    } Direction;


    // typedef struct {
    // Position *position;
    // size_t used;
    // size_t size;
    // } PositionArray;

    typedef struct tmp{
        int8_t x;
        int8_t y;
        struct tmp *next;
        struct tmp *prev;
    } PositionNode;

    typedef struct PositionArray{
        PositionNode *head;
        PositionNode *tail;
        int size;
    } PositionArray;

    typedef struct
    {
        PositionArray *snake_body;
        Position snake_head;
        Direction direction;
        int8_t  points;
    } Snake;

#endif

void initPositionArray(PositionArray *array);
PositionNode *createNode(Position position);
void addAtTail(PositionArray *array, Position position);
void delFromTail(PositionArray *array);
void addAtHead(PositionArray *array,Position position);
void delFromHead(PositionArray *array);