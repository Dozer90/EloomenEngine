#include "renderer_dx12.h"
#include "window_dx12.h"

#include "renderer_interface.h"
#include "window_interface.h"

#include "utility/defines.h"
#include "utility/colour.h"
#include "datatypes/int2.h"
#include "events/engine/events.h"

#include <d3dcompiler.h>
#include <directx/d3dx12.h>


using namespace Microsoft::WRL;
using namespace eloo;

renderer_dx12::renderer_dx12(const window_dx12* wnd) {
    if (!initilize_factory())       { return; }
    if (!initilize_device())        { return; }

    // Disable ALT+Enter fullscreen toggle
    mFactory->MakeWindowAssociation(wnd->handle(), DXGI_MWA_NO_ALT_ENTER);

    if (!create_command_queue())    { return; }
    if (!create_command_objects())  { return; }
    if (!create_swap_chain(wnd))    { return; }

    events::engine::on_window_resized.subscribe(&on_window_resized, this);

    // Root signature (empty for now)
    {
        CD3DX12_ROOT_SIGNATURE_DESC desc;
        desc.Init(0, nullptr, 0, nullptr,
                  D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

        ComPtr<ID3D10Blob> signature;
        ComPtr<ID3D10Blob> error;
        ELOO_ASSERT_FATAL(
          SUCCEEDED(D3D12SerializeRootSignature(
            &desc,
            D3D_ROOT_SIGNATURE_VERSION_1,
            &signature,
            &error
          )),
          "Unable to serialize root signature"
        );

        ELOO_ASSERT_FATAL(
          SUCCEEDED(
            mDevice->CreateRootSignature(
              0,
              signature->GetBufferPointer(),
              signature->GetBufferSize(),
              IID_PPV_ARGS(&mRootSignature)
            )),
          "Unable to create root signature"
        );
    }

    // Load precompiled shaders
    auto vsBlob = D3DReadFileToBlob(L"shaders/vertex.cso");
    auto psBlob = D3DReadFileToBlob(L"shaders/fragment.cso");

    // Describe the input layout
    D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
    };

    // Create the pipeline state object
    D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc{};
    psoDesc.InputLayout                 = { inputLayout, _countof(inputLayout) };
    psoDesc.pRootSignature              = mRootSignature.Get();
    psoDesc.VS                          = { vsBlob->GetBufferPointer(), vsBlob->GetBufferSize() };
    psoDesc.PS                          = { psBlob->GetBufferPointer(), psBlob->GetBufferSize() };
    psoDesc.RasterizerState             = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
    psoDesc.BlendState                  = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
    psoDesc.DepthStencilState           = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
    psoDesc.SampleMask                  = UINT_MAX;
    psoDesc.PrimitiveTopologyType       = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    psoDesc.NumRenderTargets            = 1;
    psoDesc.RTVFormats[0]               = DXGI_FORMAT_B8G8R8A8_UNORM;
    psoDesc.SampleDesc.Count            = 1;
    mDevice->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&mPipelineState));

    // Create a vertex buffer
    struct vertex {
        float3::values p;
        colour_t c;
    };
    vertex vertices[] = {
        { {  0.00f,  0.25f, 0.0f }, C_RED },
        { {  0.25f, -0.25f, 0.0f }, C_GREEN },
        { { -0.25f, -0.25f, 0.0f }, C_BLUE }
    };

    const UINT vertexBufferSize = sizeof(vertices);
    CD3DX12_HEAP_PROPERTIES heapProps(D3D12_HEAP_TYPE_UPLOAD);
    CD3DX12_RESOURCE_DESC bufferDesc = CD3DX12_RESOURCE_DESC::Buffer(vertexBufferSize);
    ELOO_ASSERT_FATAL(
      SUCCEEDED(
        mDevice->CreateCommittedResource(
          &heapProps,
          D3D12_HEAP_FLAG_NONE,
          &bufferDesc,
          D3D12_RESOURCE_STATE_GENERIC_READ,
          nullptr,
          IID_PPV_ARGS(&mVertexBuffer)
        )),
      "Unable to create vertex buffer"
    );

    // Copy the vertex data to the buffer
    UINT8* mappedData;
    ELOO_ASSERT_FATAL(
      SUCCEEDED(
        mVertexBuffer->Map(0, nullptr, reinterpret_cast<void**>(&mappedData))
      ),
      "Unable to map vertex buffer"
    );
    memcpy(mappedData, vertices, vertexBufferSize);
    mVertexBuffer->Unmap(0, nullptr);

    // Describe the vertex buffer view
    mVertexBufferView.BufferLocation = mVertexBuffer->GetGPUVirtualAddress();
    mVertexBufferView.StrideInBytes = sizeof(vertex);
    mVertexBufferView.SizeInBytes = vertexBufferSize;
}

renderer_dx12::~renderer_dx12() {
    mSwapChain.Reset();
    mCommandQueue.Reset();
    mDevice.Reset();
    mAdapter.Reset();
    mFactory.Reset();
}

bool renderer_dx12::initilize_factory() {
    UINT dxgiFactoryFlags = 0;

#if defined(_DEBUG)
    // Enable the D3D12 debug layer
    ComPtr<ID3D12Debug> debugController;
    if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)))) {
        debugController->EnableDebugLayer();
        dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
    } else {
        ELOO_ASSERT_FALSE("D3D12 debug layer is not available. Ensure you have the Windows SDK installed with the debug layer enabled.");
    }
#endif

    const bool success = SUCCEEDED(CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&mFactory)));
    ELOO_ASSERT_FATAL(success, "Unable to create DXGI factory");
    return success;
}

bool renderer_dx12::initilize_device() {
    // Enumerate adapters (GPUs) and create a device for the first one that supports D3D12
    ComPtr<IDXGIAdapter1> currentAdapter;
    SIZE_T bestMemory = 0;
    for (uint32_t index = 0; ; ++index) {
        if (mFactory->EnumAdapters1(index, &currentAdapter) == DXGI_ERROR_NOT_FOUND) {
            // Searched all available GPUs
            break;
        }

        DXGI_ADAPTER_DEsC adapterDesc;
        currentAdapter->GetDesc(&adapterDesc);
        if (adapterDesc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) {
            // Avoid software adapters
            continue;
        }

        // Check if this adapter has more memory than the current best and can create a D3D12 device
        if (adapterDesc.DedicatedVideoMemory > bestMemory &&
            SUCCEEDED(D3D12CreateDevice(currentAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&mDevice)))) {
            bestMemory = adapterDesc.DedicatedVideoMemory;
            mAdapter = currentAdapter;
        }
    }
    ELOO_ASSERT_FATAL(mDevice != nullptr, "Unable to find a GPU that supports DX12");
    return mDevice != nullptr;
}

bool renderer_dx12::create_command_queue() {
    D3D12_COMMAND_QUEUE_DESC commandQueueDesc{};
    commandQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
    const bool success = SUCCEEDED(mDevice->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(&mCommandQueue)));
    ELOO_ASSERT_FATAL(success, "Unable to create command queue");
    return success;
}

bool renderer_dx12::create_command_objects() {
    // Create command allocators for each frame
    for (UINT i = 0; i < BACK_BUFFER_COUNT; ++i) {
        if (FAILED(mDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&mCommandAllocator[i])))) {
            ELOO_ASSERT_FATAL_FALSE("Unable to create command allocator for frame %u", i);
            return false;
        }
    }

    // Create a command list
    if (FAILED(mDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, mCommandAllocator[0].Get(), nullptr, IID_PPV_ARGS(&mCommandList)))) {
        ELOO_ASSERT_FATAL_FALSE("Unable to create command list");
        return false;
    }
    mCommandList->Close(); // Close the command list to prepare it for execution

    // Create a fence for synchronization
    if (FAILED(mDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&mFence)))) {
        ELOO_ASSERT_FATAL_FALSE("Unable to create fence");
        return false;
    }
    mFenceEvent = CreateEvent(nullptr, false, false, nullptr);

    return true;
}

bool renderer_dx12::create_swap_chain(const window_dx12* wnd) {
    // Describe swap-chain
    DXGI_SWAP_CHAIN_DESC scDesc{};
    scDesc.Width = wnd->size().x();
    scDesc.Height = wnd->size().y();
    scDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    scDesc.BufferCount = BACK_BUFFER_COUNT;
    scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    scDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    scDesc.SampleDesc.Count = 1;
    scDesc.Flags = (mVsyncEnabled) ? DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING : DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

    // Optional: allow tearing (if supported)
    mFactory->CheckFeatureSupport(DXGI_FEATURE_PRESENT_ALLOW_TEARING, &mTearingSupported, sizeof(mTearingSupported));
    if (mTearingSupported && mTearingEnabled) {
        scDesc.Flags |= DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING;
    }

    // Create the swap-chain
    Microsoft::WRL::ComPtr<IDXGISwapChain1> swapChain1;
    if (FAILED(mFactory->CreateSwapChainForHwnd(mCommandQueue.Get(), wnd->handle(), &scDesc, nullptr, nullptr, &swapChain1))) {
        ELOO_ASSERT_FATAL_FALSE("Failed to create swap-chain");
        return false;
    }

    // Upgrade to IDXGISwapChain3
    if (FAILED(swapChain1.As(&mSwapChain))) {
        ELOO_ASSERT_FATAL_FALSE("Failed to query IDXGISwapChain3");
        return false;
    }

    return true;
}

bool renderer_dx12::create_render_target_heap() {
    // Create a descriptor heap for render targets
    D3D12_DESCRIPTOR_HEAP_DESC heapDesc{};
    heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    heapDesc.NumDescriptors = BACK_BUFFER_COUNT;
    if (FAILED(mDevice->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&mRTVHeap)))) {
        ELOO_ASSERT_FATAL_FALSE("Failed to create RTV descriptor heap");
        return false;
    }

    // Create one RTV per back-buffer
    auto rtvHandle = mRTVHeap->GetCPUDescriptorHandleForHeapStart();
    const UINT rtvStride = mDevice->GetDescriptorHandleIncrementSize(heapDesc.Type);
    for (UINT i = 0; i < BACK_BUFFER_COUNT; ++i) {
        Microsoft::WRL::ComPtr<ID3D12Resource> backBuffer;
        if (FAILED(mSwapChain->GetBuffer(i, IID_PPV_ARGS(&backBuffer)))) {
            ELOO_ASSERT_FATAL_FALSE("Failed to get swap-chain buffer %u", i);
            return false;
        }
        mBackBuffers[i] = backBuffer;
        mDevice->CreateRenderTargetView(backBuffer.Get(), nullptr, rtvHandle);
        rtvHandle.ptr += rtvStride;
    }

    return true;
}

void renderer_dx12::render(const window_interface* wnd) {
    const UINT backBufferIndex = mSwapChain->GetCurrentBackBufferIndex();

    // Reset the command allocator and command list
    mCommandAllocator[backBufferIndex]->Reset();
    mCommandList->Reset(mCommandAllocator[backBufferIndex].Get(), nullptr);

    // Transition the back buffer to render target
    D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
      mBackBuffers[backBufferIndex].Get(),
      D3D12_RESOURCE_STATE_PRESENT,
      D3D12_RESOURCE_STATE_RENDER_TARGET
    );
    mCommandList->ResourceBarrier(1, &barrier);

    // Clear the render target
    D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = mRTVHeap->GetCPUDescriptorHandleForHeapStart();
    rtvHandle.ptr += backBufferIndex * mRenderTargetDescriptorSize;
    const FLOAT clearColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    mCommandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);


    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////


    // Draw calls
    mCommandList->SetGraphicsRootSignature(mRootSignature.Get());
    mCommandList->SetPipelineState(mPipelineState.Get());

    // Set the render target
    const int4::values windowRect = wnd->viewport_rect();
    CD3DX12_VIEWPORT viewport(
        static_cast<float>(windowRect.x()),
        static_cast<float>(windowRect.y()),
        static_cast<float>(windowRect.z() - windowRect.x()),
        static_cast<float>(windowRect.w() - windowRect.y())
    );
    CD3DX12_RECT scissorRect(
        viewport.TopLeftX,
        viewport.TopLeftY,
        viewport.TopLeftX + viewport.Width,
        viewport.TopLeftY + viewport.Height
    );
    mCommandList->RSSetViewports(1, &viewport);
    mCommandList->RSSetScissorRects(1, &scissorRect);

    // IA set-up
    mCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    mCommandList->IASetVertexBuffers(0, 1, &mVertexBufferView);

    // Draw the triangle
    mCommandList->DrawInstanced(3, 1, 0, 0);


    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////


    // Transition the back buffer to present
    barrier = CD3DX12_RESOURCE_BARRIER::Transition(
        mBackBuffers[backBufferIndex].Get(),
        D3D12_RESOURCE_STATE_RENDER_TARGET,
        D3D12_RESOURCE_STATE_PRESENT
    );
    mCommandList->ResourceBarrier(1, &barrier);

    // Execute the command list and present
    ELOO_ASSERT_FATAL(
      SUCCEEDED(mCommandList->Close()),
      "Failed to close command list"
    );
    ID3D12CommandList* commandLists[] = { mCommandList.Get() };
    mCommandQueue->ExecuteCommandLists(1, commandLists);
    ELOO_ASSERT_FATAL(
      SUCCEEDED(mSwapChain->Present(1, 0)),
      "Failed to present swap-chain"
    );

    // Wait for the GPU to finish
    const int fenceIndex = (mFrameIndex + 1) % BACK_BUFFER_COUNT;
    const UINT64 fence = ++mFenceValue[fenceIndex];
    mCommandQueue->Signal(mFence.Get(), fence);
    if (mFence->GetCompletedValue() < fence) {
        mFence->SetEventOnCompletion(fence, mFenceEvent);
        WaitForSingleObject(mFenceEvent, INFINITE);
    }

    increment_frame();
}

void renderer_dx12::on_window_resized(int width, int height) {
    // 1) Wait for the GPU to finish with all frames
    const UINT64 fence = ++mFenceValue[current_frame() % BACK_BUFFER_COUNT];
    mCommandQueue->Signal(mFence.Get(), fence);
    if (mFence->GetCompletedValue() < fence) {
        mFence->SetEventOnCompletion(fence, mFenceEvent);
        WaitForSingleObject(mFenceEvent, INFINITE);
    }

    // 2) Release references to the back-buffers and RTV heap
    for (UINT i = 0; i < BACK_BUFFER_COUNT; ++i) {
        mBackBuffers[i].Reset();
    }
    mRTVHeap.Reset();

    // 3) Resize the swap-chain
    DXGI_SWAP_CHAIN_DESC desc = mSwapChain->GetDesc();
    ELOO_ASSERT_FATAL(
      SUCCEEDED(
        mSwapChain->ResizeBuffers(
        BACK_BUFFER_COUNT,
          width, height,
          desc.Format,
          desc.Flags
        )
      ),
      "Failed to resize swap-chain buffers"
    );

    // 4) Recreate RTV heap & views (same as in create_swap_chain)
    D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
    heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    heapDesc.NumDescriptors = BACK_BUFFER_COUNT;
    heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    mDevice->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&mRTVHeap));
    mRenderTargetDescriptorSize = mDevice->GetDescriptorHandleIncrementSize(
        D3D12_DESCRIPTOR_HEAP_TYPE_RTV
    );

    auto handle = mRTVHeap->GetCPUDescriptorHandleForHeapStart();
    for (UINT i = 0; i < BACK_BUFFER_COUNT; ++i) {
        Microsoft::WRL::ComPtr<ID3D12Resource> buf;
        mSwapChain->GetBuffer(i, IID_PPV_ARGS(&buf));
        mBackBuffers[i] = buf;
        mDevice->CreateRenderTargetView(buf.Get(), nullptr, handle);
        handle.ptr += mRenderTargetDescriptorSize;
    }
}
}