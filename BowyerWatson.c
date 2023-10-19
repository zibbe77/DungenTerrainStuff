#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <float.h>

#include "Graph.c"

typedef struct PointsToCumpute
{
    double x, y;
} PointsToCumpute;

// adding super triangle

PointsToCumpute pointsToCumpute = {.x = 10,
                                   .y = 10};

void BowyerWatson()
{
    int graphSize = 5;
    GraphVertex *vertex = CreatGraph(graphSize, 0);

    AddAdjacentNode(vertex, 0, 1300);
    AddAdjacentNode(vertex, 0, 20);

        // printf("test \n");

    // for (int i = 1; i < graphSize; i++)
    // {
    //     AddToHeadSlot(vertex, i, 0);
    // }
}