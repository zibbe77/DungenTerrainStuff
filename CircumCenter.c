#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <float.h>

// https://www.geeksforgeeks.org/program-find-circumcenter-triangle-2/
// Transulated from c++ to c

// C++ program to find the CIRCUMCENTER of a
// triangle

// This pair is used to store the X and Y
// coordinate of a point respectively

typedef struct pair_o
{
    double c1, c2;
} pair_o;

// #define pdd pair<double, double>

// Function to find the line given two points
void lineFromPoints(pair_o P, pair_o Q, double *a, double *b, double *c)
{
    *a = Q.c2 - P.c2;
    *b = P.c1 - Q.c1;
    *c = *a * (P.c1) + *b * (P.c2);
}

// Function which converts the input line to its
// perpendicular bisector. It also inputs the points
// whose mid-point lies on the bisector
void perpendicularBisectorFromLine(pair_o P, pair_o Q, double *a, double *b, double *c)
{
    pair_o mid_point = {
        .c1 = (P.c1 + Q.c1) / 2,
        .c2 = (P.c2 + Q.c2) / 2};

    // c = -bx + ay
    *c = -*b * (mid_point.c1) + *a * (mid_point.c2);

    double temp = *a;
    *a = -*b;
    *b = temp;
}

// Returns the intersection point of two lines
pair_o lineLineIntersection(double a1, double b1, double c1,
                            double a2, double b2, double c2)
{
    double determinant = a1 * b2 - a2 * b1;
    if (determinant == 0)
    {
        // The lines are parallel. This is simplified
        // by returning a pair of FLT_MAX
        pair_o returnValue = {.c1 = FLT_MAX,
                              .c2 = FLT_MAX};
        return returnValue;
    }
    else
    {
        double x = (b2 * c1 - b1 * c2) / determinant;
        double y = (a1 * c2 - a2 * c1) / determinant;
        pair_o returnValue = {.c1 = x,
                              .c2 = y};
        return returnValue;
    }
}

pair_o findCircumCenter(pair_o P, pair_o Q, pair_o R)
{
    // Line PQ is represented as ax + by = c
    double a, b, c;
    lineFromPoints(P, Q, &a, &b, &c);

    // Line QR is represented as ex + fy = g
    double e, f, g;
    lineFromPoints(Q, R, &e, &f, &g);

    // Converting lines PQ and QR to perpendicular
    // vbisectors. After this, L = ax + by = c
    // M = ex + fy = g
    perpendicularBisectorFromLine(P, Q, &a, &b, &c);
    perpendicularBisectorFromLine(Q, R, &e, &f, &g);

    // The point of intersection of L and M gives
    // the circumcenter
    pair_o circumcenter =
        lineLineIntersection(a, b, c, e, f, g);

    if (circumcenter.c1 == FLT_MAX &&
        circumcenter.c2 == FLT_MAX)
    {
        puts("The two perpendicular bisectors found come parallel \n");
        puts("Thus, the given points do not form a triangle and are collinear\n");
    }
    else
    {
        // printf("The circumcenter of the triangle PQR is: %f, %f  \n", circumcenter.c1, circumcenter.c2);
    }
    return circumcenter;
}

pair_o Interface(pair_o P, pair_o Q, pair_o R)
{
    return findCircumCenter(P, Q, R);
}
// int main()
// {
//     pair_o P = {.c1 = 6, .c2 = 0};
//     pair_o Q = {.c1 = 0, .c2 = 0};
//     pair_o R = {.c1 = 0, .c2 = 8};
//     findCircumCenter(P, Q, R);
//     return 0;
// }