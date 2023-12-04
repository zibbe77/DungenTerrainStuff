#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <float.h>

#include "raylib.h"
#include "raymath.h"

#include "CircumCenter.c"

typedef struct LineData
{
    Vector2 points[2];
} LineData;

typedef struct TriangleData
{
    LineData lines[3];
    Vector2 circumCenter;
} TriangleData;

int dataPointsSize = 30;

TriangleData triangleDataList[30];
bool triangleDataList_A[30];

void Setup()
{
    for (int i = 0; i < dataPointsSize; i++)
    {
        triangleDataList_A[i] = false;
    }

    // maloc
    // tridata and triboollist
}

int FindSlotTri()
{
    for (int i = 0; i < dataPointsSize; i++)
    {
        if (triangleDataList_A[i] == false)
        {
            return i;
        }
    }
    // never
    puts("tri list full \n ");
    return -1;
}

// Returns the CircumCenter
Vector2 CaluclateCircumCeneter(TriangleData tridata)
{
    pair_o P = {
        .c1 = tridata.lines[0].points[0].x,
        .c2 = tridata.lines[0].points[0].y,
    };
    pair_o Q = {
        .c1 = tridata.lines[1].points[0].x,
        .c2 = tridata.lines[1].points[0].y,
    };
    pair_o R = {
        .c1 = tridata.lines[2].points[0].x,
        .c2 = tridata.lines[2].points[0].y,
    };
    pair_o pairReturn = Interface(P, Q, R);
    Vector2 vectorReturn = {
        .x = pairReturn.c1,
        .y = pairReturn.c2,
    };
    return vectorReturn;
}

// CreatTri
int CreatTri(Vector2 v1, Vector2 v2, Vector2 v3)
{
    int triSlot = FindSlotTri();
    TriangleData triData;

    // line 1
    triData.lines[0].points[0] = v1;
    triData.lines[0].points[1] = v2;

    // line 2
    triData.lines[1].points[0] = v2;
    triData.lines[1].points[1] = v3;

    // line 3
    triData.lines[2].points[0] = v3;
    triData.lines[2].points[1] = v1;

    // Add global tri list
    triangleDataList[triSlot] = triData;
    triangleDataList_A[triSlot] = true;

    // gets circumcenter
    triangleDataList[triSlot].circumCenter = CaluclateCircumCeneter(triData);

    return triSlot;
}

bool FindLineDupLineList(LineData lineData, LineData *sampelLines, int sampelSize, int index)
{
    for (int i = 0; i < sampelSize; i++)
    {
        if (index == i)
        {
            continue;
        }
        if (Vector2Equals(lineData.points[0], sampelLines[i].points[0]) && Vector2Equals(lineData.points[1], sampelLines[i].points[1]))
        {
            return true;
        }
        if (Vector2Equals(lineData.points[0], sampelLines[i].points[1]) && Vector2Equals(lineData.points[1], sampelLines[i].points[0]))
        {
            return true;
        }
    }
    return false;
}

bool IsItCloserThanCC(TriangleData triangleData, Vector2 point)
{
    float raidus = fabs(Vector2Distance(triangleData.circumCenter, triangleData.lines[0].points[0]));
    float point_Dis = fabs(Vector2Distance(triangleData.circumCenter, point));

    if (raidus > point_Dis)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void BowyerWatson(Vector2 *pointList, int pointLength, Vector2 v1, Vector2 v2, Vector2 v3)
{
    // add super tri
    CreatTri(v1, v2, v3);

    // adds points on at a time and makes tris of it.
    for (int newP = 0; newP < pointLength; newP++)
    {
        int workingPoints[dataPointsSize];
        int workingPointsNum = 0;
        // clear
        for (int i = 0; i < dataPointsSize; i++)
        {
            workingPoints[i] = 0;
        }

        // Look att all circle save the vaild ones

        for (int i = 0; i < dataPointsSize; i++)
        {
            if (triangleDataList_A[i] == true)
            {
                if (IsItCloserThanCC(triangleDataList[i], pointList[newP]))
                {
                    workingPoints[workingPointsNum] = i;
                    workingPointsNum++;
                }
            }
        }

        // finde all unique than make tris
        int uniqueLinesNum = 0;
        int uniqueLines[dataPointsSize];

        // remove invalid list
        LineData badLinesList[dataPointsSize];
        int badLinesListNum = 0;

        for (int i = 0; i < dataPointsSize; i++)
        {
            badLinesList[i].points[0].x = 0;
            badLinesList[i].points[0].y = 0;

            badLinesList[i].points[1].x = 0;
            badLinesList[i].points[1].y = 0;
        }

        // get all lines
        for (int i = 0; i < workingPointsNum; i++)
        {
            for (int k = 0; k < 3; k++)
            {
                badLinesList[badLinesListNum].points[0].x = triangleDataList[workingPoints[i]].lines[k].points[0].x;
                badLinesList[badLinesListNum].points[0].y = triangleDataList[workingPoints[i]].lines[k].points[0].y;

                badLinesList[badLinesListNum].points[1].x = triangleDataList[workingPoints[i]].lines[k].points[1].x;
                badLinesList[badLinesListNum].points[1].y = triangleDataList[workingPoints[i]].lines[k].points[1].y;

                badLinesListNum++;
            }
        }

        // findes unique
        for (int i = 0; i < badLinesListNum; i++)
        {
            bool listbool = FindLineDupLineList(badLinesList[i], badLinesList, badLinesListNum, i);
            if (listbool == false)
            {
                uniqueLines[uniqueLinesNum] = i;
                uniqueLinesNum++;
            }
        }

        // make tris
        for (int i = 0; i < uniqueLinesNum; i++)
        {
            CreatTri(badLinesList[uniqueLines[i]].points[0], badLinesList[uniqueLines[i]].points[1], pointList[newP]);
        }

        // remove old
        for (int i = 0; i < workingPointsNum; i++)
        {
            // need to remove lines to
            triangleDataList_A[workingPoints[i]] = false;
        }
    }
}

void DebugDraw()
{
    float scale = 5;
    float offset = 5;
    for (int i = 0; i < dataPointsSize; i++)
    {
        if (triangleDataList_A[i] == true)
        {
            // triangle
            DrawLineV(Vector2Scale(Vector2AddValue(triangleDataList[i].lines[0].points[0], offset), scale), Vector2Scale(Vector2AddValue(triangleDataList[i].lines[0].points[1], offset), scale), WHITE);
            DrawLineV(Vector2Scale(Vector2AddValue(triangleDataList[i].lines[1].points[0], offset), scale), Vector2Scale(Vector2AddValue(triangleDataList[i].lines[1].points[1], offset), scale), WHITE);
            DrawLineV(Vector2Scale(Vector2AddValue(triangleDataList[i].lines[2].points[0], offset), scale), Vector2Scale(Vector2AddValue(triangleDataList[i].lines[2].points[1], offset), scale), WHITE);

            // cirkel
            // DrawCircleLines((triangleDataList[i].circumCenter.x + offset) * scale, (triangleDataList[i].circumCenter.y + offset) * scale, fabs(Vector2Distance(triangleDataList[i].circumCenter, triangleDataList[i].line1.point1)) * scale, BLUE);
        }
    }
}

void DebugPoints(Vector2 *pointlist, int pointListSize)
{
    float scale = 5;
    float offset = 5;
    for (int i = 0; i < pointListSize; i++)
    {
        DrawCircleV(Vector2Scale(Vector2AddValue(pointlist[i], offset), scale), 5, RED);
    }
}

// needs to be insde super tri
void CreatPoints(Vector2 v2, Vector2 v3, Vector2 *pointList, int pointListSize)
{
    float sizeX = (v2.x / 2) - 1;
    float sizeY = (v3.y / 2) - 1;

    for (int i = 0; i < pointListSize; i++)
    {
        pointList[i].x = GetRandomValue(1, sizeX);
        pointList[i].y = GetRandomValue(1, sizeY);
    }
}

int main()
{
    InitWindow(600, 600, "test");

    int viewPoints = 10;
    Vector2 pointList[10];

    // super tri
    Vector2 v1 = {
        .x = 0,
        .y = 0,
    };
    Vector2 v2 = {
        .x = 100,
        .y = 0,
    };
    Vector2 v3 = {
        .x = 0,
        .y = 100,
    };

    Setup();
    CreatPoints(v2, v3, pointList, 10);
    BowyerWatson(pointList, viewPoints, v1, v2, v3);

    while (!WindowShouldClose())
    {
        if (IsKeyPressed(32))
        {
            Setup();
            CreatPoints(v2, v3, pointList, 10);
            BowyerWatson(pointList, viewPoints, v1, v2, v3);
        }
        BeginDrawing();
        ClearBackground(BLACK);
        DebugDraw();
        DebugPoints(pointList, viewPoints);
        EndDrawing();
    }

    return 0;
}