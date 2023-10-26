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

typedef struct CircomCenterInfo
{
    int cordent;
    float radius;
} CircomCenterInfo;

// adding super triangle

PointsToCumpute pointsToCumpute = {.x = 10,
                                   .y = 10};

pair_o ConvertIntCordToPair(int data, int width)
{
    // printf("data %d \n", data);

    pair_o utput;
    utput.c1 = data % width;
    utput.c2 = floor(data / width);

    // printf("x %f y %f \n ", utput.c1, utput.c2);
    return utput;
}

float FindDistans(pair_o point, pair_o center)
{
    Vector2 pointV = {.x = point.c1,
                      .y = point.c2};
    Vector2 centerV = {.x = center.c1,
                       .y = center.c2};

    return Vector2Distance(pointV, centerV);
}

CircomCenterInfo CallCircumCenter(int index, TriangleData triangledata, int width)
{
    // get all points in a triangle
    pair_o P = ConvertIntCordToPair(triangledata.p1.cordent, width);
    pair_o Q = ConvertIntCordToPair(triangledata.p2.cordent, width);
    pair_o R = ConvertIntCordToPair(triangledata.p3.cordent, width);

    pair_o TempPair = Interface(P, Q, R);
    CircomCenterInfo circomCenterInfo;
    circomCenterInfo.cordent = TempPair.c2 * width + TempPair.c1;
    circomCenterInfo.radius = FindDistans(P, TempPair);

    return circomCenterInfo;
}

Vector2 ConvertIntCordToVector2(int data, int width)
{
    Vector2 utput;
    utput.x = data % width;
    utput.y = floor(data / width);
    return utput;
}

bool IsRadiusLess(float distance, int point, int newPoint, int width)
{
    Vector2 pointV = ConvertIntCordToVector2(point, width);
    Vector2 newPointV = ConvertIntCordToVector2(newPoint, width);

    float distancePtoNP = Vector2Distance(pointV, newPointV);

    if (distance > distancePtoNP)
    {
        return true;
    }
    else
    {
        return false;
    }
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

    // active triangles
    int usedTriangles;

    // for the super triangle
    usedTriangles++;

    // invalide triangles index
    int invalideTriangles[20];

    for (int i = 0; i < 20; i++)
    {
        invalideTriangles[0] = 0;
    }

    // insert point
    // AddToHeadSlot(vertex, 3, pointList[0]);
    // AddAdjacentNode(vertex, 3, 0);
    // AddAdjacentNode(vertex, 3, 4095);
    // AddAdjacentNode(vertex, 3, 63);

    // ändra 1 sen till list size
    for (int i = 0; i < 1; i++)
    {
        int prosesingTriangleIndexList[20];
        int prosesingTriNum = 0;
        for (int j = 0; j < 20; j++)
        {
            prosesingTriangleIndexList[j] = 0;
        }

        // insert point
        AddToHeadSlot(vertex, i + 3, pointList[i]);

        // findes all circum centers and exlueds non ative ones and invalid
        for (int j = 0; j < usedTriangles; j++)
        {
            if (invalideTriangles[j] == 1)
            {
                break;
            }

            CircomCenterInfo circumCenterUtPut = CallCircumCenter(0, triangleData[j], width);
            printf("C %d R %f \n", circumCenterUtPut.cordent, circumCenterUtPut.radius);

            // look if point is less then raidus add it to a list
            if (IsRadiusLess(circumCenterUtPut.radius, triangleData[j].p1.cordent, pointList[i], width))
            {
                prosesingTriangleIndexList[prosesingTriNum] = j;
                prosesingTriNum++;
            }
        }

        // Remove conestions

        // Add conestions
        for (int j = 0; j < prosesingTriNum; j++)
        {
            AddAdjacentNode(vertex, i + 3, triangleData[i].p1.cordent);
            AddAdjacentNode(vertex, i + 3, triangleData[i].p2.cordent);
            AddAdjacentNode(vertex, i + 3, triangleData[i].p3.cordent);
        }

        // Look for invalides
    }

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
    DrawCircleLines(31 * 5 + 100, 31 * 5 + 100, 44.547726 * 5, WHITE);
    EndDrawing();
    while (!WindowShouldClose())
    {
    }

    return 0;
}