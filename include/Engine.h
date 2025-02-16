#pragma once

#include <wrl.h>

#include <EASTL/unique_ptr.h>

struct IDXGIFactory4;
struct IDXGIAdapter1;
struct ID3D12Device;
struct ID3D12CommandQueue;
struct IDXGISwapChain1;

namespace eloo {
class Window;

class Engine {
public:
    Engine() = default;

    int run();

    /* RENDER PIPELINE
    
    [Local -> World]    - Iterate over all active Transform components and create their model matrix (M)
    [World -> Camera]   - Compute the view matrix (V) using Matrix4x4::lookAt(...)
    [Camera -> Clip]    - Compute the projection matrix (P) using Matrix4x4::perspective(...) or Matrix4x4::orthographic(...)
                           ~ NOTE: The last two steps can likely be simplified to just activeCamera.project()
                        - Transform each vertex position: vP_clip = P * V * M * vP_local
    [Clip -> NDC]       - Perform perspective division by dividing vP_clip by its w component (this is called Normalized Device Coordinates (NDC))
    [NDC -> Screen]     - Remap NDC positions from the ranges [-1,1] to [0,screenWidth] and [0,screenHeight]

    Result:
    ======================================================================================================================
        vP_screen = viewport * (vP_clip / vP_clip.w) = viewport * ((P * V * M * vP_local) / (P * V * M * vP_local).w)
    ======================================================================================================================
    */

    void startup();

public:
    using DXFactory4Ptr = Microsoft::WRL::ComPtr<IDXGIFactory4>;
    using DXAdapter1Ptr = Microsoft::WRL::ComPtr<IDXGIAdapter1>;
    using D3D12DevicePtr = Microsoft::WRL::ComPtr<ID3D12Device>;
    using D3D12CommandQueuePtr = Microsoft::WRL::ComPtr<ID3D12CommandQueue>;
    using D3D12CommandQueuePtr = Microsoft::WRL::ComPtr<ID3D12CommandQueue>;
    using DXSwapChainPtr = Microsoft::WRL::ComPtr<IDXGISwapChain1>;

private:
    eastl::unique_ptr<Window> mWindow;
    DXFactory4Ptr mDXFactory;
    DXAdapter1Ptr mDXAdapter;
    D3D12DevicePtr mDXDevice;
    D3D12CommandQueuePtr mDXCommandQueue;
    DXSwapChainPtr mDXSwapChain;
};
}