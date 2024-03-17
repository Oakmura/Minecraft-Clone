#include "Precompiled.h"

#include "Settings.h"

namespace def
{
    // window
    const IntVector2D g_SCREEN_SIZE = { 1280, 720 };

    // Camera
    const float g_NEAR_Z = 0.01f;
    const float g_FAR_Z = 2000.0f;

    // chunk
    const float g_CHUNK_SPHERE_RADIUS = CHUNK_HALF_SIZE * sqrt(3);

    // world
    const float g_TreeProbability = 0.02f;

    // player
    const SimpleMath::Vector3 g_PLAYER_POS = { WORLD_CENTER_XZ, WORLD_HEIGHT * CHUNK_SIZE, WORLD_CENTER_XZ };
    const float g_PLAYER_SPEED = 15.f;
    const float g_PLAYER_RADIUS = 0.5f;
    const float g_PLAYER_HEIGHT = 1.75f;
    const float g_MOUSE_SENSITIVITY = 0.003f;

    // physics
    const float g_GRAVITY = 9.8f;
}
