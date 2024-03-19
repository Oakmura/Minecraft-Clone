#pragma once

namespace def // default
{
    // window
    extern const IntVector2D g_SCREEN_SIZE;

    // Camera
    extern const float g_NEAR_Z;
    extern const float g_FAR_Z;

    // chunk
    enum { CHUNK_SIZE = 32 };
    enum { CHUNK_HALF_SIZE = CHUNK_SIZE / 2 };
    enum { CHUNK_AREA = CHUNK_SIZE * CHUNK_SIZE };
    enum { CHUNK_VOLUME = CHUNK_AREA * CHUNK_SIZE };
    extern const float g_CHUNK_SPHERE_RADIUS;

    enum { MAX_NUM_VERTEX_PER_BLOCK = 18 };
    enum { MAX_NUM_INDEX_PER_BLOCK = 36 };

    // world
    enum { WORLD_WIDTH = 16 };
    enum { WORLD_HEIGHT = 4 };
    enum { WORLD_DEPTH = WORLD_WIDTH };

    enum { WORLD_CENTER_Y = WORLD_HEIGHT * CHUNK_HALF_SIZE };
    enum { WORLD_CENTER_XZ = WORLD_WIDTH * CHUNK_HALF_SIZE };

    enum { WORLD_AREA = WORLD_WIDTH * WORLD_DEPTH };
    enum { WORLD_VOLUME = WORLD_AREA * WORLD_HEIGHT };

    enum { TREE_WIDTH = 4, TREE_HEIGHT = 8 };
    enum { TREE_HALF_WIDTH = TREE_WIDTH / 2, TREE_HALF_HEIGHT = TREE_HEIGHT / 2 };

    extern const float g_TreeProbability;

    // player
    extern const SimpleMath::Vector3 g_PLAYER_POS;
    extern const float g_PLAYER_SPEED;
    extern const float g_PLAYER_RADIUS;
    extern const float g_PLAYER_HEIGHT;
    extern const float g_MOUSE_SENSITIVITY;

    // physics
    extern const float g_GRAVITY;

    // clouds
    enum { CLOUD_HEIGHT = WORLD_HEIGHT * CHUNK_SIZE * 2 };
}
