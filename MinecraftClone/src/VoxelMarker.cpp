#include "Precompiled.h"

#include "VoxelMarker.h"

VoxelMarker::VoxelMarker()
    : mVB(nullptr)
    , mIB(nullptr)
    , mModelGPU(nullptr)
    , mInteractionModeCB(nullptr)
    , mIndexCount(0)
{
    
}

VoxelMarker::~VoxelMarker()
{
}

void VoxelMarker::Update()
{
    // update CB data for voxel handler interaction mode
}

void VoxelMarker::Render()
{
}