#pragma once

#include "renderer_interface.h"

#include "utility/defines.h"
#include "datatypes/int2.h"

#include <wrl/client.h>
#include <d3d12.h>
#include <dxgi1_6.h>

/* ====================================
RTV = Render Target View
        A descriptor that allows the GPU to render to a texture or buffer.
CBV = Constant Buffer View
        A descriptor that allows the GPU to read a constant buffer.
SRV = Shader Resource View
        A descriptor that allows the GPU to read a texture or buffer.
UAV = Unordered Access View
        A descriptor that allows the GPU to read and write to a texture or buffer.
==================================== */

using Microsoft::WRL::ComPtr;

namespace eloo {
    class window_interface;
    class window_dx12;

    class renderer_dx12 : public renderer_interface {
    public:
        explicit renderer_dx12(const window_dx12* wnd);
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

        virtual void render(const window_interface* wnd) override;

    private:
        bool initilize_factory();
        bool initilize_device();
        bool create_command_queue();
        bool create_command_objects();
        bool create_swap_chain(const window_dx12* wnd);
        bool create_render_target_heap();

        bool load_root_signature();
        bool create_default_pipeline_state();
        bool create_vertex_buffer();

        virtual void on_window_resized(int width, int height) override;

    private:
        static constexpr UINT               BACK_BUFFER_COUNT = 2;

        // Options
        bool                                mVsyncEnabled = true;
        bool                                mTearingEnabled = false;
        bool                                mTearingSupported = false;

        // DirectX 12 objects
        ComPtr<IDXGIFactory5>               mFactory;
        ComPtr<IDXGIAdapter1>               mAdapter;
        ComPtr<ID3D12Device>                mDevice;
        ComPtr<ID3D12CommandQueue>          mCommandQueue;
        ComPtr<IDXGISwapChain3>             mSwapChain;

        ComPtr<ID3D12DescriptorHeap>        mRTVHeap;

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