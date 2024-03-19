#include "Precompiled.h"

#include "Frustum.h"

Camera* Frustum::sCamera = nullptr;

float Frustum::sFactorX;
float Frustum::sFactorY;
float Frustum::sTanX;
float Frustum::sTanY;

void Frustum::SetCamera(Camera* camera)
{
    sCamera = camera;

    float halfY = camera->GetFovYInRadian() * 0.5f;
    sFactorY = 1.0f / cos(halfY);
    sTanY = tan(halfY);

    float halfX = camera->GetFovXInRadian() * 0.5f;
    sFactorX = 1.0f / cos(halfX);
    sTanX = tan(halfX);
}

bool Frustum::IsOnFrustum(const Chunk& chunk)
{
    ASSERT(sCamera != nullptr, "Frustum::IsOnFrustum : camera is not set");

    SimpleMath::Vector3 sphereVec = chunk.GetCenter() - sCamera->GetEyePos();
    
    float sz = sphereVec.Dot(sCamera->GetForward()); // near, far
    if (sCamera->GetNearZ() - def::g_CHUNK_SPHERE_RADIUS > sz || sz > sCamera->GetFarZ() + def::g_CHUNK_SPHERE_RADIUS)
    {
        return false;
    }
    
    float sy = sphereVec.Dot(sCamera->GetUp()); // top, bottom
    float dist = sFactorY * def::g_CHUNK_SPHERE_RADIUS + sz * sTanY;
    if (-dist > sy || sy > dist)
    {
        return false;
    }

    float sx = sphereVec.Dot(sCamera->GetRight()); // left, right
    dist = sFactorX * def::g_CHUNK_SPHERE_RADIUS + sz * sTanX;
    if (-dist > sx || sx > dist)
    {
        return false;
    }

    return true;
}
