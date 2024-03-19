#pragma once

#include <OpenSimplexNoise.h>

class NoiseGenerator final
{
public:
    NoiseGenerator() = delete;
    ~NoiseGenerator() = delete;
    NoiseGenerator(const NoiseGenerator& rhs) = delete;
    NoiseGenerator& operator=(const NoiseGenerator& rhs) = delete;

public:
    static double Generate(const double x, const double y);
    static double Generate(const double x, const double y, const double z);

private:
    static OpenSimplexNoise::Noise sNoise;
};

