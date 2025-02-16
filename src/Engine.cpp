#include <Engine.h>

#include <Windows.h>

#include <Generic/Utilities.h>

#include <d3d12.h>
#include <dxgi1_6.h>
#include <d3dcompiler.h>

using namespace eloo;

void Engine::startup() {
    mWindow = eastl::make_unique<Window>()


    //////////////////////////////////////////////////////////////////////////
    // Step 1: Create the factory

    ASSERT_FATAL(SUCCEEDED(CreateDXGIFactory1(IID_PPV_ARGS(&mDXFactory))),
                 "Unable to create DXGI factory!");


    //////////////////////////////////////////////////////////////////////////
    // Step 2: Search for a compatible GPU and create the Direct3D 12 device

    for (uint32_t index = 0; ; ++index) {
        if (mDXFactory->EnumAdapters1(index, &mDXAdapter) == DXGI_ERROR_NOT_FOUND) {
            // Searched for all available GPUs
            break;
        }

        DXGI_ADAPTER_DESC1 adapterDesc;
        mDXAdapter->GetDesc1(&adapterDesc);
        if (adapterDesc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) {
            // Avoid software adapters
            continue;
        }

        if (FAILED(D3D12CreateDevice(mDXAdapter.Get(), D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&mDXDevice)))) {
            continue;
        }
    }
    ASSERT_FATAL(mDXDevice != nullptr, "Could not find a GPU that supports DX12!");


    //////////////////////////////////////////////////////////////////////////
    // Step 3: Create a command queue

    D3D12_COMMAND_QUEUE_DESC commandQueueDesc{};
    commandQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
    commandQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    ASSERT_FATAL(SUCCEEDED(mDXDevice->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(&mDXCommandQueue))),
                 "Unable to create command queue!");


    //////////////////////////////////////////////////////////////////////////
    // Step 4: Create a swap chain

    DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
    swapChainDesc.Width = 1920;
    swapChainDesc.Height = 1080;
    swapChainDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.BufferCount = 2;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapChainDesc.SampleDesc.Count = 1;
    ASSERT_FATAL(SUCCEEDED(mDXFactory->CreateSwapChainForHwnd(mDXCommandQueue.Get(), mWindowHandle, &swapChainDesc, nullptr, nullptr, mDXSwapChain.Get())),
                 "Unable to create swap chain!");
}