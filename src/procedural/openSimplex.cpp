#include "openSimplex.h"

#include "log.h"

namespace neith {

OpenSimplexEnv *OpenSimplex::mOse = initOpenSimplex();
OpenSimplexGradients *OpenSimplex::mOsg = newOpenSimplexGradients(mOse, 0);

double OpenSimplex::Noise2D(double x, double y, int octaves, float persistence, float lacunarity)

{
    double res = 0;
    float amplitude = 1;
    float frequency = 1;
    for (int i = 1; i <= octaves; i++) {
        res += amplitude * noise2(mOse, mOsg, x * frequency + 5129 * i, y * frequency + 623 * i);
        amplitude *= persistence;
        frequency *= lacunarity;
    }
    return res + 1.0;
}

}  // namespace neith
