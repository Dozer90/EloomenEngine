#include "dx12/renderer_dx12.h"
#include "dx12/window_dx12.h"

#include "renderer_interface.h"
#include "window_interface.h"

#include "utility/defines.h"
#include "utility/colour.h"
#include "datatypes/int2.h"
#include "events/engine/events.h"

#include <stdexcept>
#include "rendering/mesh.h"
#include <d3dcompiler.h>
#include <directx/d3dx12.h>


using namespace Microsoft::WRL;
using namespace sge;

renderer_dx12::renderer_dx12(const window_dx12* wnd) {
    if (!initilize_factory()) { throw std::runtime_error("DXGI initialization failed"); }
    if (!initilize_device()) { throw std::runtime_error("D3D12 initialization failed"); }

    // Disable ALT+Enter fullscreen toggle
    mFactory->MakeWindowAssociation(wnd->handle(), DXGI_MWA_NO_ALT_ENTER);

    if (!create_command_queue()) { throw std::runtime_error("Command queue initialization failed"); }
    if (!create_command_objects()) { throw std::runtime_error("Command object initialization failed"); }
    if (!create_swap_chain(wnd))    { throw std::runtime_error("Swap-chain creation failed"); }
    if (!create_render_target_heap()) { throw std::runtime_error("RTV creation failed"); }



    // Root signature (empty for now)
    {
        CD3DX12_ROOT_SIGNATURE_DESC desc;
        desc.Init(0, nullptr, 0, nullptr,
                  D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

        ComPtr<ID3D10Blob> signature;
        ComPtr<ID3D10Blob> error;
        SGE_ASSERT_FATAL(
          SUCCEEDED(D3D12SerializeRootSignature(
            &desc,
            D3D_ROOT_SIGNATURE_VERSION_1,
            &signature,
            &error
          )),
          "Unable to serialize root signature"
        );

        SGE_ASSERT_FATAL(
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
    ComPtr<ID3DBlob> vsBlob; if (FAILED(D3DReadFileToBlob(L"shaders/vertex.cso", vsBlob.GetAddressOf()))) { throw std::runtime_error("Unable to read vertex shader"); }
    ComPtr<ID3DBlob> psBlob; if (FAILED(D3DReadFileToBlob(L"shaders/fragment.cso", psBlob.GetAddressOf()))) { throw std::runtime_error("Unable to read fragment shader"); }

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
    psoDesc.RTVFormats[0]               = DXGI_FORMAT_R8G8B8A8_UNORM;
    psoDesc.SampleDesc.Count            = 1;
    mDevice->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&mPipelineState));

    // Create a vertex buffer
    struct vertex {
        float p[3];
        float c[4];
    };
    vertex vertices[] = {
        { {  0.00f,  0.25f, 0.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
        { {  0.25f, -0.25f, 0.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
        { { -0.25f, -0.25f, 0.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } }
    };

    const UINT vertexBufferSize = sizeof(vertices);
    CD3DX12_HEAP_PROPERTIES heapProps(D3D12_HEAP_TYPE_UPLOAD);
    CD3DX12_RESOURCE_DESC bufferDesc = CD3DX12_RESOURCE_DESC::Buffer(vertexBufferSize);
    SGE_ASSERT_FATAL(
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
    SGE_ASSERT_FATAL(
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
    mResizeSubscription = events::engine::on_window_resized.subscribe([this](events::engine::window_resized_data data) { on_window_resized(data.width, data.height); });
}

renderer_dx12::~renderer_dx12() {
    if (mResizeSubscription) { events::engine::on_window_resized.unsubscribe(*mResizeSubscription); }

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
        SGE_ASSERT_FALSE("D3D12 debug layer is not available. Ensure you have the Windows SDK installed with the debug layer enabled.");
    }
#endif

    const bool success = SUCCEEDED(CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&mFactory)));
    SGE_ASSERT_FATAL(success, "Unable to create DXGI factory");
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

        DXGI_ADAPTER_DESC1 adapterDesc{};
        currentAdapter->GetDesc1(&adapterDesc);
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
    SGE_ASSERT_FATAL(mDevice != nullptr, "Unable to find a GPU that supports DX12");
    return mDevice != nullptr;
}

bool renderer_dx12::create_command_queue() {
    D3D12_COMMAND_QUEUE_DESC commandQueueDesc{};
    commandQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
    const bool success = SUCCEEDED(mDevice->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(&mCommandQueue)));
    SGE_ASSERT_FATAL(success, "Unable to create command queue");
    return success;
}

bool renderer_dx12::create_command_objects() {
    // Create command allocators for each frame
    for (UINT i = 0; i < BACK_BUFFER_COUNT; ++i) {
        if (FAILED(mDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&mCommandAllocator[i])))) {
            SGE_ASSERT_FATAL_FALSE("Unable to create command allocator for frame %u", i);
            return false;
        }
    }

    // Create a command list
    if (FAILED(mDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, mCommandAllocator[0].Get(), nullptr, IID_PPV_ARGS(&mCommandList)))) {
        SGE_ASSERT_FATAL_FALSE("Unable to create command list");
        return false;
    }
    mCommandList->Close(); // Close the command list to prepare it for execution

    // Create a fence for synchronization
    if (FAILED(mDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&mFence)))) {
        SGE_ASSERT_FATAL_FALSE("Unable to create fence");
        return false;
    }
    mFenceEvent = CreateEvent(nullptr, false, false, nullptr);

    return true;
}

bool renderer_dx12::create_swap_chain(const window_dx12* wnd) {
    // Describe swap-chain
    DXGI_SWAP_CHAIN_DESC1 scDesc{};
    scDesc.Width = wnd->size().x();
    scDesc.Height = wnd->size().y();
    scDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    scDesc.BufferCount = BACK_BUFFER_COUNT;
    scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    scDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    scDesc.SampleDesc.Count = 1;
    scDesc.Flags = 0;

    // Optional: allow tearing (if supported)
    mFactory->CheckFeatureSupport(DXGI_FEATURE_PRESENT_ALLOW_TEARING, &mTearingSupported, sizeof(mTearingSupported));
    if (mTearingSupported && mTearingEnabled) {
        scDesc.Flags |= DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING;
    }

    // Create the swap-chain
    Microsoft::WRL::ComPtr<IDXGISwapChain1> swapChain1;
    if (FAILED(mFactory->CreateSwapChainForHwnd(mCommandQueue.Get(), wnd->handle(), &scDesc, nullptr, nullptr, &swapChain1))) {
        SGE_ASSERT_FATAL_FALSE("Failed to create swap-chain");
        return false;
    }

    // Upgrade to IDXGISwapChain3
    if (FAILED(swapChain1.As(&mSwapChain))) {
        SGE_ASSERT_FATAL_FALSE("Failed to query IDXGISwapChain3");
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
        SGE_ASSERT_FATAL_FALSE("Failed to create RTV descriptor heap");
        return false;
    }

    // Create one RTV per back-buffer
    auto rtvHandle = mRTVHeap->GetCPUDescriptorHandleForHeapStart();
    const UINT rtvStride = mRenderTargetDescriptorSize = mDevice->GetDescriptorHandleIncrementSize(heapDesc.Type);
    for (UINT i = 0; i < BACK_BUFFER_COUNT; ++i) {
        Microsoft::WRL::ComPtr<ID3D12Resource> backBuffer;
        if (FAILED(mSwapChain->GetBuffer(i, IID_PPV_ARGS(&backBuffer)))) {
            SGE_ASSERT_FATAL_FALSE("Failed to get swap-chain buffer %u", i);
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
    mCommandList->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);
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
    SGE_ASSERT_FATAL(
      SUCCEEDED(mCommandList->Close()),
      "Failed to close command list"
    );
    ID3D12CommandList* commandLists[] = { mCommandList.Get() };
    mCommandQueue->ExecuteCommandLists(1, commandLists);
    SGE_ASSERT_FATAL(
      SUCCEEDED(mSwapChain->Present(1, 0)),
      "Failed to present swap-chain"
    );

    // Wait for the GPU to finish

    const UINT64 fence = ++mNextFenceValue;
    mCommandQueue->Signal(mFence.Get(), fence);
    if (mFence->GetCompletedValue() < fence) {
        mFence->SetEventOnCompletion(fence, mFenceEvent);
        WaitForSingleObject(mFenceEvent, INFINITE);
    }

    increment_frame();
}

void renderer_dx12::on_window_resized(int width, int height) {
    if (width <= 0 || height <= 0 || !mSwapChain) { return; }
    // 1) Wait for the GPU to finish with all frames
    const UINT64 fence = ++mNextFenceValue;
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
    DXGI_SWAP_CHAIN_DESC1 desc{}; if (FAILED(mSwapChain->GetDesc1(&desc))) { return; }
    SGE_ASSERT_FATAL(
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
namespace {
ComPtr<ID3D12Resource> upload_buffer(ID3D12Device* device, const void* data, size_t size) {
    if (!device || !data || !size) { throw std::invalid_argument("Invalid GPU buffer input"); }
    auto heap = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
    auto desc = CD3DX12_RESOURCE_DESC::Buffer(size);
    ComPtr<ID3D12Resource> resource;
    if (FAILED(device->CreateCommittedResource(&heap, D3D12_HEAP_FLAG_NONE, &desc,
        D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&resource)))) {
        throw std::runtime_error("GPU buffer allocation failed");
    }
    void* mapped = nullptr;
    D3D12_RANGE range{0, 0};
    if (FAILED(resource->Map(0, &range, &mapped))) { throw std::runtime_error("GPU buffer map failed"); }
    memcpy(mapped, data, size);
    resource->Unmap(0, nullptr);
    return resource;
}
}
vertex_buffer::id_t renderer_dx12::create_vertex_buffer(const void* data, size_t size, bool) {
    mVertexBuffers.push_back(upload_buffer(mDevice.Get(), data, size));
    return mVertexBuffers.size() - 1;
}
index_buffer::id_t renderer_dx12::create_index_buffer(const void* data, size_t size, bool) {
    mIndexBuffers.push_back(upload_buffer(mDevice.Get(), data, size));
    return mIndexBuffers.size() - 1;
}
void renderer_dx12::destroy_vertex_buffer(vertex_buffer::id_t id) {
    if (static_cast<size_t>(id) < mVertexBuffers.size()) { mVertexBuffers[id].Reset(); }
}
void renderer_dx12::destroy_index_buffer(index_buffer::id_t id) {
    if (static_cast<size_t>(id) < mIndexBuffers.size()) { mIndexBuffers[id].Reset(); }
}
// The current demo draws its own triangle. General mesh submission is unsupported.
bool renderer_dx12::register_mesh(mesh::id_t) { return false; }
bool renderer_dx12::deregister_mesh(mesh::id_t) { return false; }
bool renderer_dx12::submit_for_draw(mesh::id_t) { return false; }
bool renderer_dx12::submit_for_draw(mesh::id_t, const entity::id_t&) { return false; }
