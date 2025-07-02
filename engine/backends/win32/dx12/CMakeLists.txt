# DIRECTX 12

if (ELOO_RENDERER_BACKEND STREQUAL "DX12")

    add_library(EloomEngine_Backend_DX12 STATIC
        "${CMAKE_CURRENT_SOURCE_DIR}/src/renderer_dx12.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/window_dx12.cpp"
    )


    ###########################################
    # Include Directories

    target_include_directories(EloomEngine_Backend_DX12
        PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
            ${ELOO_PUBLIC_INCLUDE_DIRECTORIES}
            ${ELOO_PUBLIC_SYSTEM_INCLUDE_DIRECTORIES}
        PRIVATE
            ${ELOO_PRIVATE_INCLUDE_DIRECTORIES}
            ${ELOO_PRIVATE_SYSTEM_INCLUDE_DIRECTORIES}
    )


    ###########################################
    # Package Dependencies

    FetchContent_Declare(DirectXHeaders
        GIT_REPOSITORY  https://github.com/microsoft/DirectX-Headers.git
        GIT_TAG         main
    )
    FetchContent_MakeAvailable(DirectXHeaders)


    FetchContent_Declare(DXShaderCompiler
        URL             https://github.com/microsoft/DirectXShaderCompiler/releases/download/v1.8.2502/dxc_2025_02_20.zip
        URL_HASH        SHA256=70B1913A1BFCE4A3E1A5311D16246F4ECDF3A3E613ABEC8AA529E57668426F85
    )
    FetchContent_MakeAvailable(DXShaderCompiler)


    ###########################################
    # Finalization

    target_link_libraries(EloomEngine_Backend_DX12 PRIVATE Microsoft::DirectX-Headers)

    add_library(dxcompiler SHARED IMPORTED GLOBAL)
    set_target_properties(dxcompiler PROPERTIES
        IMPORTED_IMPLIB    "${DXShaderCompiler_SOURCE_DIR}/lib/dxcompiler.lib"
        IMPORTED_LOCATION  "${DXShaderCompiler_SOURCE_DIR}/bin/dxcompiler.dll"
    )

    target_link_libraries(EloomEngine PRIVATE EloomEngine_Backend_DX12)
    target_compile_definitions(EloomEngine_Backend_DX12 PUBLIC ELOO_BACKEND_DX12)


    ###########################################
    # Commands

    add_custom_command(TARGET EloomEngine_Backend_DX12 POST_BUILD
        COMMENT "Copying dxcompiler.dll for shader hot-reloading"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:dxcompiler>
            $<TARGET_FILE_DIR:EloomEngine_Backend_DX12>
    )

endif()