#include "Precompiled.h"

#include "GraphicsPsoLibrary.h"

GraphicsPsoLibrary::~GraphicsPsoLibrary()
{
    for (auto& pso : mPSOs)
    {
        delete pso.second;
    }
}

void GraphicsPsoLibrary::Add(GraphicsPSO* pso, const uint32_t key)
{
    ASSERT(mPSOs.find(key) == mPSOs.end(), "Trying to add already existing PSO");

    mPSOs.insert({ key, pso });
}

GraphicsPSO& GraphicsPsoLibrary::Get(const uint32_t key)
{
    ASSERT(mPSOs.find(key) != mPSOs.end(), "Trying to get unidentified PSOs");

    return *mPSOs[key];
}