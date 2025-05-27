#pragma once

#include "renderer_interface.h"

#include "utility/defines.h"
#include "datatypes/int2.h"

#include <wrl/client.h>
#include <d3d12.h>
#include <dxgi1_6.h>

using Microsoft::WRL::ComPtr;

namespace eloo {
    class window_interface;

    class renderer_dx12 : public renderer_interface {
    public:
        explicit renderer_dx12(window_interface* wnd);
        renderer_dx12(const renderer_dx12&) = delete;
        renderer_dx12& operator=(const renderer_dx12&) = delete;
        renderer_dx12(renderer_dx12&&) = delete;
        renderer_dx12& operator=(renderer_dx12&&) = delete;

        virtual inline constexpr const char* renderer_name() const override  { return "DirectX"; }
        virtual inline constexpr const char* platform_name() const override  { return "Windows"; }
        virtual inline constexpr const char* vendor_name()   const override  { return "Microsoft"; }
        virtual inline constexpr unsigned major_version()    const override  { return 12; }
        virtual inline constexpr unsigned minor_version()    const override  { return 0; }
        virtual inline constexpr unsigned patch_version()    const override  { return 0; }

        virtual void render() override;

    private:
        void create_swap_chain(
            ComPtr<ID3D12CommandQueue>  cmdQueue,
            unsigned int                bufferCount = 2,
            unsigned int                format      = 87 /*DXGI_FORMAT_B8G8R8A8_UNORM*/,
            int2::values                size        = int2::ZERO
        );

        virtual void on_window_resized(int width, int height) override;

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