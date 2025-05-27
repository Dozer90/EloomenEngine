# DIRECTX 12


###########################################
# Include Directories

list(APPEND ELOO_PRIVATE_INCLUDE_DIRECTORIES
    "${CMAKE_CURRENT_SOURCE_DIR}/backends/dx12/include"
)


###########################################
# Source Files

list(APPEND ELOO_RENDERING_SOURCE_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/backends/dx12/src/renderer_dx12.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/backends/dx12/src/window_dx12.cpp"
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

list(APPEND ELOO_PUBLIC_DEFINES ELOO_BACKEND_DX12)