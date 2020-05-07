#pragma once

#include <winsdkver.h>
#define _WIN32_WINNT 0x0601
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
#include <DirectXColors.h>

#pragma comment(lib,"d3dcompiler.lib")
#include <d3dcompiler.h>

// DirectXTK
#include <SimpleMath.h>
#include <Effects.h>
#include <CommonStates.h>
#include <VertexTypes.h>
#include <PrimitiveBatch.h>
#include <GeometricPrimitive.h>

// STD
#include <algorithm>
#include <exception>
#include <memory>
#include <stdexcept>
#include <vector>

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
