#include "Precompiled.h"

#include "NoiseGenerator.h"

OpenSimplexNoise::Noise NoiseGenerator::sNoise(27);

double NoiseGenerator::Generate(const double x, const double y)
{
    return sNoise.eval(x, y);
}

double NoiseGenerator::Generate(const double x, const double y, const double z)
{
    return sNoise.eval(x, y, z);
}