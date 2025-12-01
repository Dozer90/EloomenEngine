# DIRECTX 12


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