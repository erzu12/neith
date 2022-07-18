#include "voronoi.h"

#include <math.h>

#include <glm/vec3.hpp>

#include "debug.h"
#include "log.h"

double Dot(double x1, double y1, double x2, double y2) { return x1 * x2 + y1 * y2; }

double smoothstep(double edge0, double edge1, double x)
{
    if (x < edge0)
        return 0;

    if (x >= edge1)
        return 1;

    // Scale/bias into [0..1] range
    x = (x - edge0) / (edge1 - edge0);

    return x * x * (3 - 2 * x);
}

void Norm(double &x, double &y)
{
    double l = sqrt(x * x + y * y);
    x /= l;
    y /= l;
}

unsigned int shift(unsigned int a, unsigned int b, unsigned int c)
{
    c ^= b;
    c -= (b << 14) | (b >> 18);

    a ^= c;
    c -= (b << 11) | (b >> 21);

    b ^= a;
    c -= (b << 25) | (b >> 7);

    c ^= b;
    c -= (b << 16) | (b >> 16);

    a ^= c;
    c -= (b << 4) | (b >> 28);

    b ^= a;
    c -= (b << 14) | (b >> 18);

    c ^= b;
    c -= (b << 24) | (b >> 8);
    return c;
}

namespace neith {

void Hash(int x, int i, int y, int j, double &outX, double &outY)
{
    unsigned int a, b, c;
    a = b = c = 0xdeadbeefu + (2u << 2u) + 13u;

    b += (y + j);
    a += (x + i);
    c = shift(a, b, c);

    outX = ((double)c / (double)0xFFFFFFFFu) - 0.5f;
    a = b = c = 0xdeadbeefu + (2u << 2u) + 13u;

    a += (y + j);
    b += (x + i);
    c = shift(a, b, c);
    outY = ((double)c / (double)0xFFFFFFFFu) - 0.5f;
}
void CashedHash3x3(int x, int i, int y, int j, double &outX, double &outY)
{
    static double hashX[3][3];
    static double hashY[3][3];
    static int oldX;
    static int oldY;
    if (x == oldX && y == oldY) {
        outX = hashX[i + 1][j + 1];
        outY = hashY[i + 1][j + 1];
        return;
    }
    if (i == 1 && j == 1) {
        oldX = x;
        oldY = y;
    }
    Hash(x, i, y, j, outX, outY);
    hashX[i + 1][j + 1] = outX;
    hashY[i + 1][j + 1] = outY;
}
void CashedHash5x5(int x, int i, int y, int j, double &outX, double &outY)
{
    static double hashX[5][5];
    static double hashY[5][5];
    static int oldX;
    static int oldY;
    if (x == oldX && y == oldY) {
        outX = hashX[i + 2][j + 2];
        outY = hashY[i + 2][j + 2];
        return;
    }
    if (i == 2 && j == 2) {
        oldX = x;
        oldY = y;
    }
    Hash(x, i, y, j, outX, outY);
    hashX[i + 2][j + 2] = outX;
    hashY[i + 2][j + 2] = outY;
}
double SqrDist(double x1, double y1, double x2, double y2) { return (x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1); }

double Voronoi::Voronoi2D(double x, double y, float randomness)
{
    int cellX = floor(x);
    int cellY = floor(y);

    double minDist = 10.0f;

    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            double outX;
            double outY;
            CashedHash3x3(cellX, i, cellY, j, outX, outY);
            double testDist = SqrDist(x - cellX, y - cellY, 0.5f + outX * randomness + i, 0.5 + outY * randomness + j);

            if (testDist < minDist) {
                minDist = testDist;
            }
        }
    }
    return sqrt(minDist);
}

double Voronoi::VoronoiDistace2D(double x, double y, float randomness)
{
    int currentCellX = floor(x);
    int currentCellY = floor(y);

    double minDist = 10.0f;

    int mbx, mby;
    double mrx, mry;

    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            int cellX = currentCellX + i;
            int cellY = currentCellY + j;
            double rx, ry;
            CashedHash3x3(currentCellX, i, currentCellY, j, rx, ry);
            rx = 0.5 + rx * randomness + i - (x - currentCellX);
            ry = 0.5 + ry * randomness + j - (y - currentCellY);
            double testDist = Dot(rx, ry, rx, ry);

            if (testDist < minDist) {
                minDist = testDist;
                mrx = rx;
                mry = ry;
                mbx = i;
                mby = j;
            }
        }
    }
    minDist = 10.0f;
    for (int i = -2; i <= 2; i++) {
        for (int j = -2; j <= 2; j++) {
            int cellX = currentCellX + i + mbx;
            int cellY = currentCellY + j + mby;
            double rx, ry;
            CashedHash5x5(currentCellX + mbx, i, currentCellY + mby, j, rx, ry);
            rx = 0.5 + rx * randomness + i + mbx - (x - currentCellX);
            ry = 0.5 + ry * randomness + j + mby - (y - currentCellY);
            double nrx = rx - mrx;
            double nry = ry - mry;
            Norm(nrx, nry);
            double testDist = Dot(0.5 * (rx + mrx), 0.5 * (ry + mry), nrx, nry);

            if (testDist < minDist) {
                minDist = testDist;
            }
        }
    }
    return minDist;
}
}  // namespace neith
