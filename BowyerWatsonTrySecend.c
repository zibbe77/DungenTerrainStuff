#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <float.h>

#include "raylib.h"
#include "raymath.h"

#include "CircumCenter.c"

typedef struct TriangleData
{
    LineData *line1, *line2, *line3;
    Vector2 circumCenter;
} TriangleData;

typedef struct LineData
{
    Vector2 point1, point2;
} LineData;

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
}

// CreatTri
void CreatTri(Vector2 v1, Vector2 v2, Vector2 v3)
{
    int slot = FindSlotTri();
    TriangleData tridata;

    // line 1
    tridata->line1.point1 = v1;
    *tridata.line1.point2 = v2;

    // line 2
    *tridata.line2->point1 = v2;
    *tridata.line2->point2 = v3;

    // line 3
    *tridata.line3->point1 = v3;
    *tridata.line3->point2 = v1;

    triangleDataList[slot] = tridata;
}

// line function that compairs all lines to make sure we dont have duplicats
// returns True if its a dup
bool FindLineDup(LineData lineData)
{
    for (int i = 0; i < 90; i++)
    {
        if (lineDataList_A[i] == true)
        {
            if (lineData.point1 == lineDataList[i].point1 && lineData.point2 == lineDataList[i].point2)
            {
                return true;
            }
            if (lineData.point1 == lineDataList[i].point2 && lineData.point2 == lineDataList[i].point1)
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
        .c1 = tridata.line1->point1->x,
        .c2 = tridata.line1->point1->y,
    };
    pair_o Q = {
        .c1 = tridata.line2->point1->x,
        .c2 = tridata.line2->point1->y,
    };
    pair_o R = {
        .c1 = tridata.line3->point1->x,
        .c2 = tridata.line3->point1->y,
    };
    pair_o pairReturn = Interface(P, Q, R);
    Vector2 vectorReturn = {
        .x = pairReturn.c1,
        .y = pairReturn.c2,
    };
    return vectorReturn;
}

BowyerWatson()
{
    // add super tri
}