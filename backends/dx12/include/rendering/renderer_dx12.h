#pragma once

#include "utility/defines.h"
#include "core/renderer_interface.h"
#include "core/window_interface.h"
#include "datatype/int2.h"

#include <EASTL/unique_ptr.h>

#include <wrl.h>
#include <d3d12.h>
#include <dxgi1_6.h>


using namespace Microsoft::WRL;

namespace eloo {
    class renderer_dx12 : public renderer_interface {
    public:
        renderer_dx12(window_interface* wnd);
        ELOO_CTOR_DELETE_COPY(renderer_dx12);
        ELOO_CTOR_DELETE_MOVE(renderer_dx12);

        inline constexpr const char* renderer_name() const override    { return "DirectX"; }
        inline constexpr const char* platform_name() const override    { return "Windows"; }
        inline constexpr const char* vendor_name() const override      { return "Microsoft"; }
        inline constexpr unsigned int major_version() const override   { return 12; }
        inline constexpr unsigned int minor_version() const override   { return 0; }
        inline constexpr unsigned int patch_version() const override   { return 0; }

        void render() override;

    private:
        void create_swap_chain(ComPtr<ID3D12CommandQueue> cmdQueue,
                               UINT bufferCount = 2,
                               DXGI_FORMAT format = DXGI_FORMAT_B8G8R8A8_UNORM,
                               int2::values size = int2::ZERO);

        void on_window_resized(int width, int height) override;

    private:
        static constexpr UINT               FRAME_COUNT = 2;

        ComPtr<IDXGIFactory5>               mFactory;
        ComPtr<IDXGIAdapter1>               mAdapter;
        ComPtr<ID3D12Device>                mDevice;
        ComPtr<ID3D12CommandQueue>          mCommandQueue;
        ComPtr<IDXGISwapChain3>             mSwapChain;

        ComPtr<ID3D12DescriptorHeap>        mRenderTargetHeap;
        UINT                                mRenderTargetDescriptorSize = 0;

        ComPtr<ID3D12CommandAllocator>      mCommandAllocator[FRAME_COUNT];
        ComPtr<ID3D12GraphicsCommandList>   mCommandList;
        ComPtr<ID3D12Fence>                 mFence;
        UINT64                              mFenceValue[FRAME_COUNT] = {};
        HANDLE                              mFenceEvent = nullptr;

        ComPtr<ID3D12Resource>              mBackBuffers[FRAME_COUNT];

        ComPtr<ID3D12RootSignature>         mRootSignature;
        ComPtr<ID3D12PipelineState>         mPipelineState;
        ComPtr<ID3D12Resource>              mVertexBuffer;
        D3D12_VERTEX_BUFFER_VIEW            mVertexBufferView;
    };
}