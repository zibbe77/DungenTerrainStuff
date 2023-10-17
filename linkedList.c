#include <math.h>
#include <stdlib.h>

typedef struct Linked_Node
{
    int data;
    struct Linked_Node *next;
} Linked_Node;

struct Linked_Node *CreatLinkedList(int data)
{
    Linked_Node *head = (Linked_Node *)malloc(sizeof(Linked_Node));
    head->data = data;
    head->next = NULL;

    return head;
}
// printf("test 1 \n");

void AddNodeLinkedList(Linked_Node *head, int data)
{
    Linked_Node *tempPonter = head->next;
    Linked_Node *newLink = (Linked_Node *)malloc(sizeof(Linked_Node));
    head->next = newLink;

    newLink->next = tempPonter;

    newLink->data = data;
}

void RemoveNodeLinkedList(Linked_Node *head, int intput)
{
    Linked_Node *tempPonterCurrent = head->next;
    Linked_Node *tempPonterPrevious = head;
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

void FreeLinkedList(Linked_Node *head)
{
    Linked_Node *tempPonterNext = head->next;
    free(head);
    while (1)
    {
        Linked_Node *tempPonterCurrent = tempPonterNext;
        tempPonterNext = tempPonterNext->next;
        free(tempPonterCurrent);
        if (tempPonterNext == NULL)
        {
            break;
        }
    }
}

void PrintLinkedList(Linked_Node *head)
{
    printf("%d \n", head->data);
    Linked_Node *tempPonter = head->next;
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