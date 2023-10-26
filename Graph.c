#include <math.h>
#include <stdlib.h>

#include "linkedList.c"

typedef struct GraphVertex
{
    Linked_Node *head;
} GraphVertex;

GraphVertex *CreatGraph(int size, int data)
{
    GraphVertex *vertex = (GraphVertex *)malloc(sizeof(GraphVertex) * size);
    vertex[0].head = CreatLinkedList(data);

    for (int i = 1; i < size; i++)
    {
        vertex[i].head = NULL;
    }

    return vertex;
}

void RemoveGraph(GraphVertex *vertex, int size)
{
    for (int i = 0; i < size; i++)
    {
        FreeLinkedList(vertex[i].head);
    }

    free(vertex);
}

GraphVertex *ChangeSizeHeadSlots(GraphVertex *oldVertex, int oldSize, int newSize)
{
    GraphVertex *vertex = (GraphVertex *)malloc(sizeof(GraphVertex) * newSize);

    for (int i = 0; i < newSize; i++)
    {
        vertex[i].head = oldVertex[i].head;
    }

    RemoveGraph(oldVertex, oldSize);
    return vertex;
}

void AddToHeadSlot(GraphVertex *vertex, int index, int data)
{
    vertex[index].head = CreatLinkedList(data);
}

void AddAdjacentNode(GraphVertex *vertex, int index, int data)
{
    AddNodeLinkedList(vertex[index].head, data);
}

void RemoveAdjacentNode(GraphVertex *vertex, int index, int data)
{
    RemoveNodeLinkedList(vertex[index].head, data);
}

int ReaddataOne(GraphVertex *vertex, int index, int step)
{
    return ReaddataAtStep(vertex[index].head, step);
}

ReadReturn ReaddataOne_Un(GraphVertex *vertex, int index, int step)
{
    return ReadData_Un(vertex[index].head, step);
}

void PrintGraph(GraphVertex *vertex, int size)
{
    for (int i = 0; i < size; i++)
    {
        printf("Rad %d ------------- \n", i);
        PrintLinkedList(vertex[i].head);
    }
}