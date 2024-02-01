#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <dxgidebug.h>
#include <directxtk/SimpleMath.h>

using namespace DirectX;
using DirectX::SimpleMath::Vector2;
using DirectX::SimpleMath::Vector3;
using DirectX::SimpleMath::Vector4;
using DirectX::SimpleMath::Matrix;
using DirectX::SimpleMath::Quaternion;

#include <functional>

#include <iostream>

#include <memory>
#include <vector>
#include <string>
#include <tchar.h>
#include <windows.h>

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

#include "Logger.h"
#include "Utils/UtilHeader.h"