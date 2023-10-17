#include <math.h>
#include <stdlib.h>

typedef struct Linked_Node
{
    int data;
    struct Linked_Node *next;
} Linked_Node;

Linked_Node *linkedHead;

void CreatLinkedList(int data)
{
    // array = (float *)malloc(width * height * sizeof(float)
    linkedHead = (Linked_Node *)malloc(sizeof(Linked_Node));
    linkedHead->data = data;
    linkedHead->next = NULL;
}
// printf("test 1 \n");

void AddNodeLinkedList(int data)
{
    Linked_Node *tempPonter = linkedHead->next;
    Linked_Node *newLink = (Linked_Node *)malloc(sizeof(Linked_Node));
    linkedHead->next = newLink;

    newLink->next = tempPonter;

    newLink->data = data;
}

void RemoveNodeLinkedList(int intput)
{
    Linked_Node *tempPonterCurrent = linkedHead->next;
    Linked_Node *tempPonterPrevious = linkedHead;
    while (1)
    {
        if (tempPonterCurrent->data == intput)
        {
            tempPonterPrevious->next = tempPonterCurrent->next;
            free(tempPonterCurrent);
            break;
        }

        tempPonterPrevious = tempPonterCurrent;
        tempPonterCurrent = tempPonterCurrent->next;

        if (tempPonterCurrent == NULL)
        {
            printf("somting went roung linkted list\n");
            break;
        }
    }
}

void PrintLinkedList()
{
    printf("%d \n", linkedHead->data);
    Linked_Node *tempPonter = linkedHead->next;
    while (1)
    {
        printf("%d \n", tempPonter->data);
        tempPonter = tempPonter->next;

        if (tempPonter == NULL)
        {
            break;
        }
    }
}