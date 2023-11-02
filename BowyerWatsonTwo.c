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
    struct TriangleData *parent;
    int index;
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

int FindSlotLine()
{
    for (int i = 0; i < 90; i++)
    {
        if (lineDataList_A[i] == false)
        {
            return i;
        }
    }
    // never
    puts("line list full \n ");
    return -1;
}

// CreatTri
int CreatTri(Vector2 v1, Vector2 v2, Vector2 v3)
{
    int triSlot = FindSlotTri();
    TriangleData triData;

    // line 1
    int slot = FindSlotLine();
    triData.line1 = &lineDataList[slot];
    lineDataList_A[slot] = true;

    triData.line1->index = slot;

    triData.line1->point1 = v1;
    triData.line1->point2 = v2;

    triData.line1->parent = &triangleDataList[triSlot];

    // line 2
    slot = FindSlotLine();
    triData.line2 = &lineDataList[slot];
    lineDataList_A[slot] = true;

    triData.line2->index = slot;

    triData.line2->point1 = v2;
    triData.line2->point2 = v3;

    triData.line2->parent = &triangleDataList[triSlot];

    // line 3
    slot = FindSlotLine();
    triData.line3 = &lineDataList[slot];
    lineDataList_A[slot] = true;

    triData.line2->index = slot;

    triData.line3->point1 = v3;
    triData.line3->point2 = v1;

    triData.line3->parent = &triangleDataList[triSlot];

    // Add global tri list
    triangleDataList[triSlot] = triData;
    triangleDataList_A[triSlot] = true;

    // gets circumcenter
    triangleDataList[triSlot].circumCenter = CaluclateCircumCeneter(triData);

    return triSlot;
}

void RemoveTri()
{
}

// line function that compairs all lines to make sure we dont have duplicats
// returns True if its a dup
bool FindLineDupList(LineData lineData, TriangleData *parent)
{
    for (int i = 0; i < 90; i++)
    {
        // printf("index %d ; A %d \n ", i, lineDataList_A[i]);
        if (lineDataList_A[i] == true)
        {
            // if (parent == lineData.parent)
            // {
            //     continue;
            // }
            if (lineData.index == i)
            {
                continue;
            }
            // printf("tru \n");
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

// returns True if its a dup
bool FindLineDup(LineData lineData, int *invalidLines, int invalidLinesNum)
{
    for (int i = 0; i < invalidLinesNum; i++)
    {
        if (lineData.index == i)
        {
            continue;
        }
        if (Vector2Equals(lineData.point1, lineDataList[invalidLines[i]].point1) && Vector2Equals(lineData.point2, lineDataList[invalidLines[i]].point2))
        {
            return true;
        }
        if (Vector2Equals(lineData.point1, lineDataList[invalidLines[i]].point2) && Vector2Equals(lineData.point2, lineDataList[invalidLines[i]].point1))
        {
            return true;
        }
    }
    return false;
}

bool IsItCloserThanCC(TriangleData triangleData, Vector2 point)
{
    float raidus = fabs(Vector2Distance(triangleData.circumCenter, triangleData.line1->point1));
    float point_Dis = fabs(Vector2Distance(triangleData.circumCenter, point));

    // printf("radius %f point_Dif %f", raidus, point_Dis);
    if (raidus > point_Dis)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void BowyerWatson(Vector2 *pointList, int pointLength)
{
    // add super tri
    Vector2 v1 = {
        .x = 0,
        .y = 0,
    };
    Vector2 v2 = {
        .x = 80,
        .y = 0,
    };
    Vector2 v3 = {
        .x = 0,
        .y = 80,
    };
    CreatTri(v1, v2, v3);

    // v1.x = 0;
    // v1.y = 0;

    // v2.x = 60;
    // v2.y = 0;

    // v3.x = 0;
    // v3.y = 60;
    // CreatTri(v1, v2, v3);
    for (int newP = 0; newP < pointLength; newP++)
    {
        int workingPoints[10];
        int workingPointsNum = 0;
        // clear
        for (int i = 0; i < 10; i++)
        {
            workingPoints[i] = 0;
        }

        // Look att all circle save the vaild ones

        for (int i = 0; i < 30; i++)
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

        int tempSave[10];
        int tempSaveNum = 0;
        // clear
        for (int i = 0; i < 10; i++)
        {
            tempSave[i] = 0;
        }

        // creat new triangles
        for (int i = 0; i < workingPointsNum; i++)
        {
            int triNum;
            tempSave[tempSaveNum] = CreatTri(pointList[newP], triangleDataList[workingPoints[i]].line1->point1, triangleDataList[workingPoints[i]].line2->point1);
            tempSaveNum++;

            tempSave[tempSaveNum] = CreatTri(pointList[newP], triangleDataList[workingPoints[i]].line2->point1, triangleDataList[workingPoints[i]].line3->point1);
            tempSaveNum++;

            tempSave[tempSaveNum] = CreatTri(pointList[newP], triangleDataList[workingPoints[i]].line3->point1, triangleDataList[workingPoints[i]].line1->point1);
            tempSaveNum++;
        }

        int invalidLines[10];
        int invalidLinesNum = 0;
        for (int i = 0; i < 10; i++)
        {
            invalidLines[i] = 0;
        }

        // remove invalid

        //  remove duplicats tri
        for (int i = 0; i < workingPointsNum; i++)
        {
            int sides = 0;
            // line 1
            bool removeBool = FindLineDupList(*triangleDataList[workingPoints[i]].line1, &triangleDataList[workingPoints[i]]);
            if (removeBool == true)
            {
                sides++;
            }
            // line 2
            removeBool = FindLineDupList(*triangleDataList[workingPoints[i]].line2, &triangleDataList[workingPoints[i]]);
            if (removeBool == true)
            {
                sides++;
            }
            // line 3
            removeBool = FindLineDupList(*triangleDataList[workingPoints[i]].line3, &triangleDataList[workingPoints[i]]);
            if (removeBool == true)
            {
                sides++;
            }
            if (sides == 3)
            {
                triangleDataList_A[workingPoints[i]] = false;
                printf("removde tri dup \n");
            }
        }

        // new

        int workingLinesList[10];
        int workingLinesListNum = 0;

        // get all lines
        for (int i = 0; i < workingPointsNum; i++)
        {
            workingLinesList[workingLinesListNum] = triangleDataList[workingPoints[workingPointsNum]].line1->index;
            workingLinesListNum++;

            workingLinesList[workingLinesListNum] = triangleDataList[workingPoints[workingPointsNum]].line2->index;
            workingLinesListNum++;

            workingLinesList[workingLinesListNum] = triangleDataList[workingPoints[workingPointsNum]].line3->index;
            workingLinesListNum++;
        }
        // printf("workinglinellistNum %d \n", workingLinesListNum);

        // finde dup lines
        printf(" start \n");
        for (int i = 0; i < workingLinesListNum; i++)
        {
            printf("index i %d \n", i);
            bool listbool = FindLineDup(lineDataList[workingLinesList[i]], workingLinesList, workingLinesListNum);
            if (listbool == true)
            {
                invalidLines[invalidLinesNum] = workingLinesList[i];
                invalidLinesNum++;
                printf("invalid lines %d \n", invalidLinesNum);
            }
        }
        printf(" stop \n");

        // printf("invailde line Num %d \n", invalidLinesNum);

        for (int i = 0; i < tempSaveNum; i++)
        {
            // printf("invalide lines %d \n", i);
            bool removeBool = FindLineDup(*triangleDataList[tempSave[i]].line1, invalidLines, invalidLinesNum);
            if (removeBool == true)
            {
                printf("line 1 \n");
                // need to remove lines to
                triangleDataList_A[tempSave[i]] = false;
            }
            removeBool = FindLineDup(*triangleDataList[tempSave[i]].line2, invalidLines, invalidLinesNum);
            if (removeBool == true)
            {
                printf("line 2 \n");
                // need to remove lines to
                triangleDataList_A[tempSave[i]] = false;
            }
            removeBool = FindLineDup(*triangleDataList[tempSave[i]].line3, invalidLines, invalidLinesNum);
            if (removeBool == true)
            {
                // need to remove lines to
                printf("line 3 \n");
                triangleDataList_A[tempSave[i]] = false;
            }
        }

        // remove old
        for (int i = 0; i < workingPointsNum; i++)
        {
            // need to remove lines to
            printf("remove old  \n");
            triangleDataList_A[workingPoints[i]] = false;
        }
    }
}

void DebugDraw()
{
    float scale = 5;
    float offset = 5;
    for (int i = 0; i < 30; i++)
    {
        if (triangleDataList_A[i] == true)
        {
            // triangle
            DrawLineV(Vector2Scale(Vector2AddValue(triangleDataList[i].line1->point1, offset), scale), Vector2Scale(Vector2AddValue(triangleDataList[i].line1->point2, offset), scale), WHITE);
            DrawLineV(Vector2Scale(Vector2AddValue(triangleDataList[i].line2->point1, offset), scale), Vector2Scale(Vector2AddValue(triangleDataList[i].line2->point2, offset), scale), WHITE);
            DrawLineV(Vector2Scale(Vector2AddValue(triangleDataList[i].line3->point1, offset), scale), Vector2Scale(Vector2AddValue(triangleDataList[i].line3->point2, offset), scale), WHITE);

            // cirkel
            DrawCircleLines((triangleDataList[i].circumCenter.x + offset) * scale, (triangleDataList[i].circumCenter.y + offset) * scale, fabs(Vector2Distance(triangleDataList[i].circumCenter, triangleDataList[i].line1->point1)) * scale, BLUE);
        }
    }
}

int main()
{
    InitWindow(600, 600, "test");

    Vector2 pointList[5];

    Vector2 temp = {
        .x = 20,
        .y = 20,
    };
    pointList[0] = temp;

    Vector2 temp2 = {
        .x = 20,
        .y = 30,
    };
    pointList[1] = temp2;

    Vector2 temp3 = {
        .x = 10,
        .y = 35,
    };
    pointList[2] = temp3;

    Setup();
    BowyerWatson(pointList, 3);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);
        DebugDraw();

        EndDrawing();
    }

    return 0;
}