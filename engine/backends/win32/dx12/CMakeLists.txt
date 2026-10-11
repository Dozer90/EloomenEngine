# DIRECTX 12

if (SGE_RENDERER_BACKEND STREQUAL "DX12")

    add_library(ShineGE_Backend_DX12 STATIC
        "${CMAKE_CURRENT_SOURCE_DIR}/src/renderer_dx12.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/window_dx12.cpp"
    )


    ###########################################
    # Include Directories

    target_include_directories(ShineGE_Backend_DX12
        PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
            ${SGE_PUBLIC_INCLUDE_DIRECTORIES}
            ${SGE_PUBLIC_SYSTEM_INCLUDE_DIRECTORIES}
        PRIVATE
            ${SGE_PRIVATE_INCLUDE_DIRECTORIES}
            ${SGE_PRIVATE_SYSTEM_INCLUDE_DIRECTORIES}
    )

    target_compile_definitions(ShineGE_Backend_DX12
        PRIVATE NOMINMAX ${SGE_PRIVATE_DEFINES} ${SGE_PUBLIC_DEFINES}
    )


    ###########################################
    # Package Dependencies

    FetchContent_Declare(DirectXHeaders
        GIT_REPOSITORY  https://github.com/microsoft/DirectX-Headers.git
        GIT_TAG         461ef5138bd30922b44fbad77c4e47662812d2d5
    )
    FetchContent_MakeAvailable(DirectXHeaders)


    FetchContent_Declare(DXShaderCompiler
        URL             https://github.com/microsoft/DirectXShaderCompiler/releases/download/v1.8.2502/dxc_2025_02_20.zip
        URL_HASH        SHA256=70B1913A1BFCE4A3E1A5311D16246F4ECDF3A3E613ABEC8AA529E57668426F85
    )
    FetchContent_MakeAvailable(DXShaderCompiler)


    ###########################################
    # Finalization

    target_link_libraries(ShineGE_Backend_DX12 PUBLIC Microsoft::DirectX-Headers PRIVATE d3d12 dxgi d3dcompiler)

    add_library(dxcompiler SHARED IMPORTED GLOBAL)
    set_target_properties(dxcompiler PROPERTIES
        IMPORTED_IMPLIB    "${dxshadercompiler_SOURCE_DIR}/lib/x64/dxcompiler.lib"
        IMPORTED_LOCATION  "${dxshadercompiler_SOURCE_DIR}/bin/x64/dxcompiler.dll"
    )

    target_link_libraries(sge PRIVATE ShineGE_Backend_DX12)
    target_compile_definitions(ShineGE_Backend_DX12 PUBLIC SGE_BACKEND_DX12)


    ###########################################
    # Commands


endif()