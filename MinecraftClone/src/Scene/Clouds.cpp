#include "Precompiled.h"

#include "Clouds.h"
#include "Generators/NoiseGenerator.h"
#include "Utils/Hasher.h"

Clouds::Clouds()
{
    GraphicsEngine& ge = GraphicsEngine::GetInstance();
    ID3D11Device& device = ge.GetDevice();

    std::vector<uint8_t> cloudData(def::WORLD_AREA * def::CHUNK_SIZE * def::CHUNK_SIZE, 0);
    for (int x = 0; x < def::WORLD_WIDTH * def::CHUNK_SIZE; ++x)
    {
        for (int z = 0; z < def::WORLD_DEPTH * def::CHUNK_SIZE; ++z)
        {
            if (NoiseGenerator::Generate(0.13 * x, 0.13 * z) >= 0.2)
            {
                cloudData[x + def::WORLD_WIDTH * def::CHUNK_SIZE * z] = 1;
            }
        }
    }

    mVertices.reserve(def::WORLD_AREA * def::CHUNK_AREA * 6 * 3);
    mIndices.reserve(def::WORLD_AREA * def::CHUNK_AREA * 6 * 3);
    buildVertices(cloudData);

    D3D11Utils::CreateVertexBuffer(device, mVertices, &mVB);
    D3D11Utils::CreateIndexBuffer(device, mIndices, &mIB);
    mIndexCount = UINT(mIndices.size());

    mCloudsCB.GetCPU().Time = 0.0f;
    mCloudsCB.GetCPU().CloudScale = 25;
    mCloudsCB.GetCPU().WorldCenterXZ = def::WORLD_CENTER_XZ;
    D3D11Utils::CreateConstantBuffer(device, mCloudsCB.GetCPU(), &mCloudsCB.GetGPU());
}

Clouds::~Clouds()
{
    RELEASE_COM(mVB);
    RELEASE_COM(mIB);
}

void Clouds::Update(const float dt)
{
    mCloudsCB.GetCPU().Time += dt;
    D3D11Utils::UpdateBuffer(GraphicsEngine::GetInstance().GetDeviceContext(), mCloudsCB.GetCPU(), mCloudsCB.GetGPU());
}

void Clouds::Render()
{
    GraphicsEngine& ge = GraphicsEngine::GetInstance();
    GraphicsResourceLibrary& grl = ge.GetResourceLibrary();
    ID3D11DeviceContext& context = ge.GetDeviceContext();

    context.IASetInputLayout(&grl.GetIL(Hasher::Hash("pos")));
    context.VSSetShader(&grl.GetVS(Hasher::Hash("cloud")), nullptr, 0);
    context.PSSetShader(&grl.GetPS(Hasher::Hash("cloud")), nullptr, 0);

    UINT offset = 0;
    UINT stride = sizeof(CloudsVertex);

    context.IASetVertexBuffers(0, 1, &mVB, &stride, &offset);
    context.IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);

    mCloudsCB.UseOn(eShader::Vertex, 5);

    context.DrawIndexed(mIndexCount, 0, 0);
}

void Clouds::buildVertices(const std::vector<uint8_t>& cloudData)
{
    int y = def::CLOUD_HEIGHT;
    int width = def::WORLD_WIDTH * def::CHUNK_SIZE;
    int depth = def::WORLD_DEPTH * def::CHUNK_SIZE;

    std::set<int> visited;
    for (int z = 0; z < depth; ++z)
    {
        for (int x = 0; x < width; ++x) 
        {
            int idx = x + width * z;
            if (cloudData[idx] == 0 || visited.count(idx))
            {
                continue;
            }

            // find number of continuous quads along x
            int xCount = 1;
            idx = (x + xCount) + width * z;
            while ((x + xCount) < width && cloudData[idx] && !visited.count(idx))
            {
                xCount += 1;
                idx = (x + xCount) + width * z;
            }

            // find the number of continuous quads along z for each x
            std::vector<int> zCountList;
            int zCount;
            for (int ix = 0; ix < xCount; ++ix)
            {
                zCount = 1;
                idx = (x + ix) + width * (z + zCount);
                while ((z + zCount) < depth && cloudData[idx] && !visited.count(idx))
                {
                    zCount += 1;
                    idx = (x + ix) + width * (z + zCount);
                }
                zCountList.push_back(zCount);
            }

            // find min count z to form a large quad
            if (zCountList.empty())
            {
                zCount = 1;
            }
            else
            {
                zCount = *(std::min_element(zCountList.begin(), zCountList.end()));
            }

            // mark all unit quads of the large quad as visited
            for (int ix = 0; ix < xCount; ++ix)
            {
                for (int iz = 0; iz < zCount; ++iz)
                {
                    visited.insert((x + ix) + width * (z + iz));
                }
            }

            mVertices.push_back({ (float)x, (float)y, (float)z });
            mVertices.push_back({ (float)(x + xCount), (float)y, (float)(z + zCount) });
            mVertices.push_back({ (float)(x + xCount), (float)y, (float)z });
            mVertices.push_back({ (float)x, (float)y, (float)(z + zCount) });

            static uint32_t curIdx = 0;
            mIndices.push_back(curIdx);
            mIndices.push_back(curIdx + 1);
            mIndices.push_back(curIdx + 2);
            mIndices.push_back(curIdx);
            mIndices.push_back(curIdx + 3);
            mIndices.push_back(curIdx + 1);
            curIdx += 4;
        }
    }
}
