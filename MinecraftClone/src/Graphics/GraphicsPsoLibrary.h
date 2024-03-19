#pragma once

#include "Graphics/GraphicsPSO.h"

class GraphicsPsoLibrary final
{
public:
    GraphicsPsoLibrary() = default;
    ~GraphicsPsoLibrary() = default;
    GraphicsPsoLibrary(const GraphicsPsoLibrary& rhs) = delete;
    GraphicsPsoLibrary& operator=(const GraphicsPsoLibrary& rhs) = delete;

    void Add(GraphicsPSO* pso, const uint32_t key);
    GraphicsPSO& Get(const uint32_t key);

private:
    std::unordered_map<uint32_t, GraphicsPSO*> mPSOs;
};