#pragma once

enum { CHUNK_SIZE = 32 };
enum { CHUNK_HALF_SIZE = CHUNK_SIZE / 2 };
enum { CHUNK_AREA = CHUNK_SIZE * CHUNK_SIZE };
enum { CHUNK_VOLUME = CHUNK_AREA * CHUNK_SIZE };

enum { MAX_NUM_VERTEX_PER_VOXEL = 18 };
enum { MAX_NUM_INDEX_PER_VOXEL = 36 };

enum { WORLD_WIDTH = 20 };
enum { WORLD_HEIGHT = 4 };
enum { WORLD_DEPTH = WORLD_WIDTH };

enum { WORLD_CENTER_Y = WORLD_HEIGHT * CHUNK_HALF_SIZE };
enum { WORLD_CENTER_XZ = WORLD_WIDTH * CHUNK_HALF_SIZE };

enum { WORLD_AREA = WORLD_WIDTH * WORLD_DEPTH };
enum { WORLD_VOLUME = WORLD_AREA * WORLD_HEIGHT };

enum { TREE_WIDTH = 4, TREE_HEIGHT = 8 };
enum { TREE_HALF_WIDTH = TREE_WIDTH / 2, TREE_HALF_HEIGHT = TREE_HEIGHT / 2 };

static constexpr float sTreeProbability = 0.02f;

// player
static constexpr float sMOUSE_SENSITIVITY = 0.003f;
static constexpr float sPLAYER_SPEED = 15.f;
static constexpr float sPLAYER_RADIUS = 0.5f;
static constexpr float sPLAYER_HEIGHT = 1.75f;
