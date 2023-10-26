#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <float.h>

#include "raylib.h"
#include "raymath.h"

#include "Graph.c"
#include "CircumCenter.c"

typedef struct PointsToCumpute
{
    double x, y;
} PointsToCumpute;

typedef struct TrianglInturnalData
{
    int cordent;
    int index;
} TrianglInturnalData;

typedef struct TriangleData
{
    struct TrianglInturnalData p1, p2, p3;

} TriangleData;

typedef struct triangleInfo
{
    pair_o center;
    pair_o firstPoint;
    pair_o secendPoint;
    pair_o tiridPoint;
} triangleInfo;

// adding super triangle

PointsToCumpute pointsToCumpute = {.x = 10,
                                   .y = 10};

pair_o ConvertIntCordToPair(int data, int width)
{
    printf("data %d \n", data);

    pair_o utput;
    utput.c1 = data % width;
    utput.c2 = floor(data / width);

    printf("x %f y %f \n ", utput.c1, utput.c2);
    return utput;
}

triangleInfo CallCircumCenter(int index, GraphVertex *vertex, int width)
{
    // get all points in a triangle
    pair_o P = ConvertIntCordToPair(ReaddataOne(vertex, index, 0), width);
    pair_o Q = ConvertIntCordToPair(ReaddataOne(vertex, index, 1), width);
    pair_o R = ConvertIntCordToPair(ReaddataOne(vertex, index, 2), width);

    // input points to pair_o
    // pair_o P = {.c1 = 11, .c2 = 0};
    // pair_o Q = {.c1 = 0, .c2 = 0};
    // pair_o R = {.c1 = 0, .c2 = 8};
    triangleInfo interfaceUtput;
    interfaceUtput.center = Interface(P, Q, R);
    interfaceUtput.firstPoint = P;
    interfaceUtput.secendPoint = Q;
    interfaceUtput.tiridPoint = R;

    return interfaceUtput;
}

void DebugDrawTriangle(GraphVertex *vertex, int graphSize, int width)
{
    int scale = 5;

    for (int i = 0; i < graphSize; i++)
    {
        if (vertex[i].head == NULL)
        {
            break;
        }
        ReadReturn read = ReaddataOne_Un(vertex, i, 0);
        int headPos = read.data;

        for (int j = 0; j < 20; j++)
        {
            ReadReturn readReturn = ReaddataOne_Un(vertex, i, j);
            DrawCircle((readReturn.data % width) * scale + 100, floor(readReturn.data / width) * scale + 100, 5, RED);
            DrawLine((headPos % width) * scale + 100, floor(headPos / width) * scale + 100, (readReturn.data % width) * scale + 100, floor(readReturn.data / width) * scale + 100, WHITE);
            if (readReturn.head == NULL)
            {
                break;
            }
        }
    }
}

void BowyerWatson(int width, int *pointList, int listSize)
{
    int graphSize = 5;

    // creats super triangle
    //  slot 0
    GraphVertex *vertex = CreatGraph(graphSize, 0);
    AddAdjacentNode(vertex, 0, 4095);
    AddAdjacentNode(vertex, 0, 63);

    // slot 1
    AddToHeadSlot(vertex, 1, 4095);
    AddAdjacentNode(vertex, 1, 0);
    AddAdjacentNode(vertex, 1, 63);

    // slot 2
    AddToHeadSlot(vertex, 2, 63);
    AddAdjacentNode(vertex, 2, 0);
    AddAdjacentNode(vertex, 2, 4095);

    // adds triangle
    TriangleData triangleData[20];
    TriangleData triangleDatatemp = {.p1.cordent = 0, .p1.index = 0, .p2.cordent = 4095, .p2.index = 1, .p3.cordent = 63, .p3.index = 2};
    triangleData[0] = triangleDatatemp;

    triangleInfo callCircumCenterUtput = CallCircumCenter(0, vertex, width);

    // insert point
    AddToHeadSlot(vertex, 3, pointList[0]);
    AddAdjacentNode(vertex, 3, 0);
    AddAdjacentNode(vertex, 3, 4095);
    AddAdjacentNode(vertex, 3, 63);

    // debug
    DebugDrawTriangle(vertex, graphSize, width);

    // draw Point
    DrawCircle((pointList[0] % width) * 5 + 100, floor(pointList[0] / width) * 5 + 100, 5, GREEN);
}

int main(int argc, char const *argv[])
{
    InitWindow(1920, 1080, "test");
    BeginDrawing();

    int pointList[5];
    pointList[0] = 655;

    BowyerWatson(64, pointList, 5);
    EndDrawing();
    while (!WindowShouldClose())
    {
    }

    return 0;
}