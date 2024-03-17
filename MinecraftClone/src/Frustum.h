#pragma once

#include "Camera.h"

class Frustum final
{
public:
    Frustum() = delete;
    ~Frustum() = delete;
    Frustum(const Frustum& rhs) = delete;
    Frustum& operator=(const Frustum& rhs) = delete;

public:
    static void SetCamera(Camera* camera);
    static bool IsOnFrustum(const Chunk& chunk);

private:
    static Camera* sCamera;

    static float sFactorX;
    static float sFactorY;
    static float sTanX;
    static float sTanY;
};

