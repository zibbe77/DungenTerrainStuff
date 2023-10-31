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
    Vector2 point1, point2;
} LineData;

typedef struct TriangleData
{
    LineData *line1, *line2, *line3;
    Vector2 circumCenter;
} TriangleData;

TriangleData triangleDataList[30];
LineData lineDataList[90];
Vector2 vertexList[90];

bool triangleDataList_A[30];
bool lineDataList_A[90];

void Setup()
{
    for (int i = 0; i < 30; i++)
    {
        triangleDataList_A[i] = false;
    }
    for (int i = 0; i < 90; i++)
    {
        lineDataList_A[i] = false;
    }
}

int FindSlotTri()
{
    for (int i = 0; i < 30; i++)
    {
        if (triangleDataList_A[i] == true)
        {
            return i;
        }
    }
    // never
    return -1;
}

int FindSlotLine()
{
    for (int i = 0; i < 90; i++)
    {
        if (lineDataList_A[i] == true)
        {
            return i;
        }
    }
    // never
    return -1;
}

// CreatTri
void CreatTri(Vector2 v1, Vector2 v2, Vector2 v3)
{
    int triSlot = FindSlotTri();
    TriangleData triData;

    // line 1
    int slot = FindSlotLine();
    triData.line1 = &lineDataList[slot];
    lineDataList_A[slot] = true;

    triData.line1->point1 = v1;
    triData.line1->point2 = v2;

    // line 2
    slot = FindSlotLine();
    triData.line2 = &lineDataList[slot];
    lineDataList_A[slot] = true;

    triData.line2->point1 = v2;
    triData.line2->point2 = v3;

    // line 3
    slot = FindSlotLine();
    triData.line3 = &lineDataList[slot];
    lineDataList_A[slot] = true;

    triData.line3->point1 = v3;
    triData.line3->point2 = v1;

    // Add global tri list
    triangleDataList[triSlot] = triData;
    triangleDataList_A[triSlot] = true;
}

// line function that compairs all lines to make sure we dont have duplicats
// returns True if its a dup
bool FindLineDup(LineData lineData)
{
    for (int i = 0; i < 90; i++)
    {
        if (lineDataList_A[i] == true)
        {

            if (Vector2Equals(lineData.point1, lineDataList[i].point1) && Vector2Equals(lineData.point2, lineDataList[i].point2))
            {
                return true;
            }
            if (Vector2Equals(lineData.point1, lineDataList[i].point2) && Vector2Equals(lineData.point2, lineDataList[i].point1))
            {
                return true;
            }
        }
    }
    return false;
}

// Returns the CircumCenter
Vector2 CaluclateCircumCeneter(TriangleData tridata)
{
    pair_o P = {
        .c1 = tridata.line1->point1.x,
        .c2 = tridata.line1->point1.y,
    };
    pair_o Q = {
        .c1 = tridata.line2->point1.x,
        .c2 = tridata.line2->point1.y,
    };
    pair_o R = {
        .c1 = tridata.line3->point1.x,
        .c2 = tridata.line3->point1.y,
    };
    pair_o pairReturn = Interface(P, Q, R);
    Vector2 vectorReturn = {
        .x = pairReturn.c1,
        .y = pairReturn.c2,
    };
    return vectorReturn;
}

void BowyerWatson(Vector2 *pointList)
{
    // add super tri
    Vector2 v1 = {
        .x = 0,
        .y = 0,
    };
    Vector2 v2 = {
        .x = 60,
        .y = 0,
    };
    Vector2 v3 = {
        .x = 0,
        .y = 30,
    };
    CreatTri(v1, v2, v3);
}

void DebugDraw()
{
    float scale = 5;
    float offset = 50;
    for (int i = 0; i < 30; i++)
    {
        if (triangleDataList_A[i] == true)
        {
            DrawLineV(triangleDataList[i].line1->point1, triangleDataList[i].line1->point2, WHITE);
            DrawLineV(triangleDataList[i].line2->point1, triangleDataList[i].line2->point2, WHITE);
            DrawLineV(triangleDataList[i].line3->point1, triangleDataList[i].line3->point2, WHITE);
        }
    }
}

int main()
{
    InitWindow(600, 600, "test");

    Vector2 pointList[5];

    Vector2 temp = {
        .x = 5,
        .y = 5,
    };
    pointList[0] = temp;

    BowyerWatson(pointList);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);

        EndDrawing();
    }

    return 0;
}