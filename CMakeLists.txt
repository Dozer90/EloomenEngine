cmake_minimum_required(VERSION 3.28)
project(EloomEngine LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

include(FetchContent)

message("============= Active config: ${CMAKE_BUILD_TYPE} =============")


###########################################
# Platform selection

if(WIN32)
    message("============= Windows platform detected =============")
    set(DEFAULT_BACKEND "DX12")
elseif(UNIX)
    message("============= Unix platform detected =============")
    set(DEFAULT_BACKEND "Vulkan")
elseif(APPLE)
    message("============= Apple platform detected =============")
    set(DEFAULT_BACKEND "Metal")
else()
    message("============= Unable to determine default platform =============")
    set(DEFAULT_BACKEND "")
endif()

set(RENDERER_BACKEND
    ${DEFAULT_BACKEND}
    CACHE STRING "Select the renderer backend to use. Options: DX12, Vulkan, OpenGL, Metal"
)

set_property(CACHE RENDERER_BACKEND PROPERTY STRINGS DX12 Vulkan OpenGL Metal None)

if(RENDERER_BACKEND STREQUAL "DX12")
    set(BACKEND_PRE_INCLUDE "backends/dx12/_pre.cmake")
    set(BACKEND_POST_INCLUDE "backends/dx12/_post.cmake")
else()
    message(FATAL_ERROR "============= Unsupported backend chosen =============")
endif()


###########################################
# Include Directories

set(INCLUDE_DIRECTORIES
    "include"
)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    #list(APPEND ELOO_INCLUDE_DIRECTORY)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    #list(APPEND ELOO_INCLUDE_DIRECTORY)
endif()


###########################################
# Source Files

set(DATATYPE_SOURCE_FILES
    "src/datatypes/float2.cpp"
    "src/datatypes/float3.cpp"
    "src/datatypes/float4.cpp"
    "src/datatypes/int2.cpp"
    "src/datatypes/int3.cpp"
    "src/datatypes/int4.cpp"
    "src/datatypes/quaternion.cpp"
    "src/datatypes/matrix2x2.cpp"
    "src/datatypes/matrix3x3.cpp"
    "src/datatypes/matrix4x4.cpp"
)

set(RENDERING_SOURCE_FILES
    "src/rendering/camera.cpp"
    "src/rendering/renderer.cpp"
    "src/rendering/window.cpp"
)

set (MATH_SOURCE_FILES
    "src/maths/interpolation.cpp"
)

set (UTILITY_SOURCE_FILES
    "src/utility/imgui_ext.cpp"
    "src/utility/raycast.cpp"
    "src/utility/spherecast.cpp"
)

set(SOURCES
    ${DATATYPE_SOURCE_FILES}
    ${RENDERING_SOURCE_FILES}
    ${MATH_SOURCE_FILES}
    ${UTILITY_SOURCE_FILES}
    "src/main.cpp"
    )

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    #list(APPEND SOURCES)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    #list(APPEND SOURCES)
endif()


###########################################
# Package Dependencies

# Box2D
FetchContent_Declare(Box2D
    GIT_REPOSITORY https://github.com/erincatto/box2d.git
    GIT_TAG v3.0.0
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)

# SFML
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)

# EASTL
FetchContent_Declare(EASTL
    GIT_REPOSITORY https://github.com/electronicarts/EASTL.git
    GIT_TAG 3.21.23
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)

#Dear ImGui
FetchContent_Declare(ImGui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG v1.91.4
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)

FetchContent_MakeAvailable(Box2D SFML EASTL ImGui)


###########################################
# Defines

set(PRIVATE_DEFINES "NOMINMAX")
set(PUBLIC_DEFINES "")

# DEBUG Config
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    #list(APPEND PRIVATE_DEFINES)
    list(APPEND PUBLIC_DEFINES
        DEBUG_BUILD
        ELOO_ASSERTS_ENABLED
        ELOO_IMGUI_IS_ENABLED
    )
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    #list(APPEND PRIVATE_DEFINES)
    list(APPEND PUBLIC_DEFINES
        RELEASE_BUILD
        ELOO_ASSERTS_ENABLED
    )
endif()

include(TestBigEndian)
TEST_BIG_ENDIAN(IsBigEndianResult)
if(IsBigEndianResult)
    list(APPEND PUBLIC_DEFINES IS_BIG_ENDIAN)
endif()


############################################
# Finalization

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${BACKEND_PRE_INCLUDE}")
    include("${CMAKE_CURRENT_SOURCE_DIR}/${BACKEND_PRE_INCLUDE}")
endif()

add_executable(EloomEngine ${SOURCES})

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${BACKEND_POST_INCLUDE}")
    include("${CMAKE_CURRENT_SOURCE_DIR}/${BACKEND_POST_INCLUDE}")
endif()

target_include_directories(EloomEngine PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/backends)
target_include_directories(EloomEngine PRIVATE ${box2d_SOURCE_DIR}/include)
target_include_directories(EloomEngine PRIVATE ${imgui_SOURCE_DIR})

target_link_libraries(EloomEngine PRIVATE Box2D)
target_link_libraries(EloomEngine PRIVATE sfml-graphics sfml-window sfml-system sfml-audio)
target_link_libraries(EloomEngine PRIVATE EASTL)
target_link_libraries(EloomEngine PRIVATE ImGui)

message("============= Private compile definitions: ${PRIVATE_DEFINES} =============")
target_compile_definitions(EloomEngine PRIVATE ${PRIVATE_DEFINES})

message("============= Public compile definitions: ${PUBLIC_DEFINES} =============")
target_compile_definitions(EloomEngine PUBLIC ${PUBLIC_DEFINES})

include_directories(${INCLUDE_DIRECTORIES})

target_compile_features(EloomEngine PRIVATE cxx_std_20)