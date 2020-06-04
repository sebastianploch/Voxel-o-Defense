#pragma once

#include <winsdkver.h>
#define _WIN32_WINNT 0x0602
#include <sdkddkver.h>

// Use the C++ standard templated min/max
#define NOMINMAX

// DirectX apps don't need GDI
#define NODRAWTEXT
#define NOGDI
#define NOBITMAP

// Include <mcx.h> if you need this
#define NOMCX

// Include <winsvc.h> if you need this
#define NOSERVICE

// WinHelp is deprecated
#define NOHELP

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

// ComPtr
#include <wrl/client.h>

// Direct3D
#include <d3d11_1.h>
#include <dxgi1_2.h>
#include <d3dcompiler.h>
#include <DirectXColors.h>

// DirectXTK
#include <Audio.h>
#include <Mouse.h>
#include <Keyboard.h>
#include <SimpleMath.h>
#include <VertexTypes.h>
#include <CommonStates.h>
#include <DDSTextureLoader.h>

// STD
#include <map>
#include <string>
#include <vector>
#include <memory>
#include <locale>
#include <codecvt>
#include <fstream>
#include <algorithm>
#include <exception>
#include <stdexcept>

// JSON nlohmann library
#include <nlohmann/json.hpp>

// Common Structs
#include "Commons.h"
#include "JSONLoader.h"

// Sound Handling
#include "Sound.h"

// Input Handling
#include "InputState.h"

namespace DX
{
    inline void ThrowIfFailed(HRESULT hr)
    {
        if (FAILED(hr))
        {
            // Set a breakpoint on this line to catch DirectX API errors
            throw std::exception();
        }
    }
}
