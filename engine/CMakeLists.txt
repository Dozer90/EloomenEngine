option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

include(CMakePrintHelpers)
include(FetchContent)

message("============= Active config: ${CMAKE_BUILD_TYPE} =============")


###########################################
# Platform selection

if(WIN32)
    message("============= Windows platform detected =============")
    set(ELOO_DEFAULT_BACKEND "DX12")
elseif(UNIX)
    message("============= Unix platform detected =============")
    set(ELOO_DEFAULT_BACKEND "Vulkan")
elseif(APPLE)
    message("============= Apple platform detected =============")
    set(ELOO_DEFAULT_BACKEND "Metal")
else()
    message("============= Unable to determine default platform =============")
    set(ELOO_DEFAULT_BACKEND "")
endif()

set(ELOO_RENDERER_BACKEND
    ${ELOO_DEFAULT_BACKEND}
    CACHE STRING "Select the renderer backend to use. Options: DX12, Vulkan, OpenGL, Metal"
)

set_property(CACHE ELOO_RENDERER_BACKEND PROPERTY STRINGS DX12 Vulkan OpenGL Metal None)

if(ELOO_RENDERER_BACKEND STREQUAL "DX12")
    set(ELOO_BACKEND_PRE_INCLUDE "backends/dx12/dx12_pre.cmake")
    set(ELOO_BACKEND_POST_INCLUDE "backends/dx12/dx12_post.cmake")
else()
    message(FATAL_ERROR "============= Unsupported backend chosen =============")
endif()


###########################################
# Include Directories

set(ELOO_PUBLIC_INCLUDE_DIRECTORIES
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    "${CMAKE_CURRENT_SOURCE_DIR}/backends/core/interfaces"
)
set(ELOO_PRIVATE_INCLUDE_DIRECTORIES)
set(ELOO_PUBLIC_SYSTEM_INCLUDE_DIRECTORIES)
set(ELOO_PRIVATE_SYSTEM_INCLUDE_DIRECTORIES)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    #list(APPEND ELOO_INCLUDE_DIRECTORY)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    #list(APPEND ELOO_INCLUDE_DIRECTORY)
endif()


###########################################
# Source Files

set(ELOO_DATATYPE_SOURCE_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/datatypes/float2.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/datatypes/float3.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/datatypes/float4.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/datatypes/int2.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/datatypes/int3.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/datatypes/int4.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/datatypes/quaternion.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/datatypes/matrix2x2.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/datatypes/matrix3x3.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/datatypes/matrix4x4.cpp"
)

set(ELOO_RENDERING_SOURCE_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/rendering/camera.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/rendering/renderer.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/rendering/window.cpp"
)

set (ELOO_MATH_SOURCE_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/maths/interpolation.cpp"
)

set (ELOO_UTILITY_SOURCE_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/utility/imgui_ext.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/utility/raycast.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/utility/spherecast.cpp"
)

set(ELOO_SOURCE_FILES
    ${ELOO_DATATYPE_SOURCE_FILES}
    ${ELOO_RENDERING_SOURCE_FILES}
    ${ELOO_MATH_SOURCE_FILES}
    ${ELOO_UTILITY_SOURCE_FILES}
)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    #list(APPEND SOURCE_FILES)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    #list(APPEND SOURCE_FILES)
endif()


###########################################
# Package Dependencies

# Box2D
FetchContent_Declare(Box2D
    GIT_REPOSITORY https://github.com/erincatto/box2d.git
    GIT_TAG v3.0.0
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL)
FetchContent_MakeAvailable(Box2D)
list(APPEND ELOO_PUBLIC_SYSTEM_INCLUDE_DIRECTORIES ${box2d_SOURCE_DIR}/include)

# EASTL
FetchContent_Declare(EASTL
    GIT_REPOSITORY https://github.com/electronicarts/EASTL.git
    GIT_TAG 3.21.23
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL)
FetchContent_MakeAvailable(EASTL)
list(APPEND ELOO_PUBLIC_SYSTEM_INCLUDE_DIRECTORIES ${EASTL_SOURCE_DIR}/include)

#Dear ImGui
FetchContent_Declare(ImGui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG v1.91.4
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL)
FetchContent_MakeAvailable(ImGui)
list(APPEND ELOO_PUBLIC_SYSTEM_INCLUDE_DIRECTORIES ${imgui_SOURCE_DIR})


###########################################
# Defines

set(ELOO_PRIVATE_DEFINES "NOMINMAX")
set(ELOO_PUBLIC_DEFINES "")

# DEBUG Config
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    #list(APPEND ELOO_PRIVATE_DEFINES)
    list(APPEND ELOO_PUBLIC_DEFINES
        DEBUG_BUILD
        ELOO_ASSERTS_ENABLED
        ELOO_IMGUI_IS_ENABLED
    )
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    #list(APPEND ELOO_PRIVATE_DEFINES)
    list(APPEND ELOO_PUBLIC_DEFINES
        RELEASE_BUILD
        ELOO_ASSERTS_ENABLED
    )
endif()

include(TestBigEndian)
TEST_BIG_ENDIAN(IsBigEndianResult)
if(IsBigEndianResult)
    list(APPEND ELOO_PUBLIC_DEFINES IS_BIG_ENDIAN)
endif()


############################################
# Finalization

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${ELOO_BACKEND_PRE_INCLUDE}")
    include("${CMAKE_CURRENT_SOURCE_DIR}/${ELOO_BACKEND_PRE_INCLUDE}")
endif()

add_library(EloomEngine STATIC ${ELOO_SOURCE_FILES})
set_target_properties(EloomEngine PROPERTIES
  ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
)

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${ELOO_BACKEND_POST_INCLUDE}")
    include("${CMAKE_CURRENT_SOURCE_DIR}/${ELOO_BACKEND_POST_INCLUDE}")
endif()

target_include_directories(EloomEngine
    PUBLIC  ${ELOO_PUBLIC_INCLUDE_DIRECTORIES}
    PRIVATE ${ELOO_PRIVATE_INCLUDE_DIRECTORIES}
)
target_include_directories(EloomEngine
    SYSTEM PUBLIC  ${ELOO_PUBLIC_SYSTEM_INCLUDE_DIRECTORIES}
    SYSTEM PRIVATE ${ELOO_PRIVATE_SYSTEM_INCLUDE_DIRECTORIES}
)

target_link_libraries(EloomEngine
    PUBLIC
        Box2D
        EASTL
        ImGui
)

target_compile_definitions(EloomEngine
    PUBLIC  ${ELOO_PUBLIC_DEFINES}
    PRIVATE ${ELOO_PRIVATE_DEFINES}
)

if(MSVC)
    target_compile_options(EloomEngine
        PRIVATE /external:W0)
endif()