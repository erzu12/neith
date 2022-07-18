#pragma once

#include "openSimplex2F.h"

namespace neith {

class OpenSimplex {
public:
    static double Noise2D(double x, double y, int octaves, float persistence, float lacunarity);

private:
    static OpenSimplexEnv *mOse;
    static OpenSimplexGradients *mOsg;
};
}  // namespace neith
