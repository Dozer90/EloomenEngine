# DIRECTX 12


###########################################
# Source Files

list(APPEND RENDERING_SOURCE_FILES
    "backend/dx12/src/rendering/renderer_dx12.cpp"
    "backend/dx12/src/rendering/window_dx12.cpp"
)


###########################################
# Package Dependencies

FetchContent_Declare(DirectXHeaders
    GIT_REPOSITORY https://github.com/microsoft/DirectX-Headers.git
    GIT_TAG main
)
FetchContent_MakeAvailable(DirectXHeaders)


###########################################
# Definitions

list(APPEND PUBLIC_DEFINES ELOO_BACKEND_DX12)