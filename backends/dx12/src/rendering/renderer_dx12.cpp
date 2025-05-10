#include "rendering/renderer.h"
#include "rendering/window.h"

#include "events/engine/events.h"

#include "utility/colour.h"

#include <d3dcompiler.h>
#include <directx/d3dx12.h>


using namespace eloo;

renderer_dx12::renderer_dx12(window_interface* wnd) {

    //////////////////////////////////////////////////////////////////////////
    // Step 1: Create the factory

    ELOO_ASSERT_FATAL(
      SUCCEEDED(CreateDXGIFactory1(IID_PPV_ARGS(&mFactory))),
      "Unable to create DXGI factory"
    );


    //////////////////////////////////////////////////////////////////////////
    // Step 2: Search for a compatible GPU and create the Direct3D 12 device

    for (uint32_t index = 0; ; ++index) {
        if (mFactory->EnumAdapters1(index, &mAdapter) == DXGI_ERROR_NOT_FOUND) {
            // Searched for all available GPUs
            break;
        }

        DXGI_ADAPTER_DESC1 adapterDesc;
        mAdapter->GetDesc1(&adapterDesc);
        if (adapterDesc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) {
            // Avoid software adapters
            continue;
        }

        if (SUCCEEDED(
          D3D12CreateDevice(
            mAdapter.Get(),
            D3D_FEATURE_LEVEL_12_0,
            IID_PPV_ARGS(&mDevice))
          )
        ) {
            break;
        }
    }
    ELOO_ASSERT_FATAL(mDevice != nullptr, "Unable to find a GPU that supports DX12");


    //////////////////////////////////////////////////////////////////////////
    // Step 3: Create a command queue

    D3D12_COMMAND_QUEUE_DESC commandQueueDesc{};
    commandQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
    ELOO_ASSERT_FATAL(
      SUCCEEDED(
        mDevice->CreateCommandQueue(
          &commandQueueDesc,
          IID_PPV_ARGS(&mCommandQueue)
        )
      ),
      "Unable to create command queue"
    );


    //////////////////////////////////////////////////////////////////////////
    // Step 4: Create a swap chain

    create_swap_chain(mCommandQueue, 2, DXGI_FORMAT_B8G8R8A8_UNORM, wnd->size());


    //////////////////////////////////////////////////////////////////////////
    // Step 5: Create a command allocator

    for (UINT i = 0; i < FRAME_COUNT; ++i) {
      ELOO_ASSERT_FATAL(
        SUCCEEDED(
          mDevice->CreateCommandAllocator(
            D3D12_COMMAND_LIST_TYPE_DIRECT,
            IID_PPV_ARGS(&mCommandAllocator[i])
          )
        ),
        "Unable to create command allocator"
      );
    }


    //////////////////////////////////////////////////////////////////////////
    // Step 6: Create a command list

    ELOO_ASSERT_FATAL(
      SUCCEEDED(
        mDevice->CreateCommandList(
          0,
          D3D12_COMMAND_LIST_TYPE_DIRECT,
          mCommandAllocator[0].Get(),
          nullptr,
          IID_PPV_ARGS(&mCommandList)
        )),
      "Unable to create command list"
    );


    //////////////////////////////////////////////////////////////////////////
    // Step 7: Create a fence

    ELOO_ASSERT_FATAL(
      SUCCEEDED(
        mDevice->CreateFence(
          0,
          D3D12_FENCE_FLAG_NONE,
          IID_PPV_ARGS(&mFence)
        )),
      "Unable to create fence"
    );
    mFenceEvent = CreateEvent(nullptr, false, false, nullptr);


    //////////////////////////////////////////////////////////////////////////
    // Step 8: Bind the window resize callback

    events::engine::on_window_resized.subscribe(&on_window_resized, this);


    //////////////////////////////////////////////////////////////////////////
    // Step 9: Other stuff

    // Root signature (empty for now)
    {
        CD3DX12_ROOT_SIGNATURE_DESC desc;
        desc.Init(0, nullptr, 0, nullptr,
                  D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

        D3D12BlobPtr signature;
        D3D12BlobPtr error;
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

void renderer_dx12::shutdown() {
    mSwapChain.Reset();
    mCommandQueue.Reset();
    mDevice.Reset();
    mAdapter.Reset();
    mFactory.Reset();
    mWindow->hide();
}

void renderer_dx12::create_swap_chain(D3D12CommandQueuePtr cmdQueue,
                                 UINT bufferCount,
                                 DXGI_FORMAT format,
                                 int2::values size) {
    // Describe swap-chain
    DXGI_SWAP_CHAIN_DESC1 scDesc{};
    scDesc.Width = width;
    scDesc.Height = height;
    scDesc.Format = format;
    scDesc.BufferCount = bufferCount;
    scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    scDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    scDesc.SampleDesc.Count = 1;

    // Optional: allow tearing if supported
    BOOL allowTearing = FALSE;
    if (SUCCEEDED(mFactory->CheckFeatureSupport(DXGI_FEATURE_PRESENT_ALLOW_TEARING, &allowTearing, sizeof(allowTearing))) && allowTearing) {
        scDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING;
    }

    // Create the swap-chain
    Microsoft::WRL::ComPtr<IDXGISwapChain1> sc1;
    ELOO_ASSERT_FATAL(
      SUCCEEDED(
        mFactory->CreateSwapChainForHwnd(
          cmdQueue.Get(),
          mWindow->handle(),
          &scDesc, nullptr, nullptr,
          &sc1
        )
      ),
      "Failed to create swap-chain"
    );

    // Disable ALT+Enter fullscreen toggle
    mFactory->MakeWindowAssociation(
      mWindow->handle(),
      DXGI_MWA_NO_ALT_ENTER
    );

    // Upgrade to IDXGISwapChain3
    ELOO_ASSERT_FATAL(
      SUCCEEDED(sc1.As(&mSwapChain)),
      "Failed to query IDXGISwapChain3"
    );

    // Create an RTV descriptor heap
    D3D12_DESCRIPTOR_HEAP_DESC heapDesc{};
    heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    heapDesc.NumDescriptors = bufferCount;
    heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    ELOO_ASSERT_FATAL(
      SUCCEEDED(mDevice->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&mRenderTargetHeap))),
      "Failed to create RTV heap"
    );
    mRenderTargetDescriptorSize = mDevice->GetDescriptorHandleIncrementSize(
      D3D12_DESCRIPTOR_HEAP_TYPE_RTV
    );

    // Create one RTV per back-buffer
    auto rtvHandle = mRenderTargetHeap->GetCPUDescriptorHandleForHeapStart();
    for (UINT i = 0; i < bufferCount; ++i) {
        Microsoft::WRL::ComPtr<ID3D12Resource> backBuffer;
        ELOO_ASSERT_FATAL(
          SUCCEEDED(mSwapChain->GetBuffer(i, IID_PPV_ARGS(&backBuffer))),
          "Failed to get swap-chain buffer"
        );
        mBackBuffers[i] = backBuffer;
        mDevice->CreateRenderTargetView(backBuffer.Get(), nullptr, rtvHandle);
        rtvHandle.ptr += mRenderTargetDescriptorSize;
    }
}

void renderer_dx12::render() {
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
    D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = mRenderTargetHeap->GetCPUDescriptorHandleForHeapStart();
    rtvHandle.ptr += backBufferIndex * mRenderTargetDescriptorSize;
    const FLOAT clearColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    mCommandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);


    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////


    // Draw calls
    mCommandList->SetGraphicsRootSignature(mRootSignature.Get());
    mCommandList->SetPipelineState(mPipelineState.Get());

    // Set the render target
    CD3DX12_VIEWPORT viewport(0.0f, 0.0f, static_cast<float>(mWindow->width()), static_cast<float>(mWindow->height()));
    CD3DX12_RECT scissorRect(0, 0, static_cast<LONG>(mWindow->width()), static_cast<LONG>(mWindow->height()));
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
    const int fenceIndex = (mFrameIndex + 1) % FRAME_COUNT;
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
    const UINT64 fence = ++mFenceValue[current_frame() % FRAME_COUNT];
    mCommandQueue->Signal(mFence.Get(), fence);
    if (mFence->GetCompletedValue() < fence) {
        mFence->SetEventOnCompletion(fence, mFenceEvent);
        WaitForSingleObject(mFenceEvent, INFINITE);
    }

    // 2) Release references to the back-buffers and RTV heap
    for (UINT i = 0; i < FRAME_COUNT; ++i) {
        mBackBuffers[i].Reset();
    }
    mRenderTargetHeap.Reset();

    // 3) Resize the swap-chain
    DXGI_SWAP_CHAIN_DESC1 desc = mSwapChain->GetDesc1();
    ELOO_ASSERT_FATAL(
      SUCCEEDED(
        mSwapChain->ResizeBuffers(
        FRAME_COUNT,
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
    heapDesc.NumDescriptors = FRAME_COUNT;
    heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    mDevice->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&mRenderTargetHeap));
    mRenderTargetDescriptorSize = mDevice->GetDescriptorHandleIncrementSize(
        D3D12_DESCRIPTOR_HEAP_TYPE_RTV
    );

    auto handle = mRenderTargetHeap->GetCPUDescriptorHandleForHeapStart();
    for (UINT i = 0; i < FRAME_COUNT; ++i) {
        Microsoft::WRL::ComPtr<ID3D12Resource> buf;
        mSwapChain->GetBuffer(i, IID_PPV_ARGS(&buf));
        mBackBuffers[i] = buf;
        mDevice->CreateRenderTargetView(buf.Get(), nullptr, handle);
        handle.ptr += mRenderTargetDescriptorSize;
    }
}
}