#pragma once

#include "Generators/GeometryGenerator.h"
#include "Graphics/ConstantBuffer.h"
#include "Core/GraphicsEngine.h"
#include "BlockHandler.h"

class BlockMarker final
{
public:
    BlockMarker();
    ~BlockMarker();
    BlockMarker(const BlockMarker& rhs) = delete;
    BlockMarker& operator=(const BlockMarker& rhs) = delete;
     
    void Update(const BlockHandler& blockHandler);
    void Render(const BlockHandler& blockHandler);

private:
    ID3D11Buffer* mVB;
    ID3D11Buffer* mIB;
    UINT mIndexCount;

    ConstantBuffer<ModelCB> mModelMatrixCB;
    ConstantBuffer<InteractionModeCB> mInteractionModeCB;
};

