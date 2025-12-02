# DIRECTX 12


list(APPEND ELOO_SOURCE_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/backend/dx12/window.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/backend/dx12/renderer.cpp"
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