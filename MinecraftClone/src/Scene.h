#pragma once

#include "Chunk.h"
#include "Managers/GraphicsResourceManager.h"

class Scene final
{
    friend class Renderer;
    
public:
    Scene(GraphicsResourceManager& GRM);

private:
    Chunk mChunk;
};

