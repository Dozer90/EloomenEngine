option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

include(CMakePrintHelpers)
include(FetchContent)

message("============= Active config: ${CMAKE_BUILD_TYPE} =============")


###########################################
# Automations

file(GLOB_RECURSE SGE_AUTOMATIONS
    CONFIGURE_DEPENDS
    "${CMAKE_SOURCE_DIR}/cmake/automations/*/automate.cmake"
)

foreach(SGE_AUTOMATION IN LISTS SGE_AUTOMATIONS)
    get_filename_component(SGE_CURRENT_AUTOMATION_DIR  ${SGE_AUTOMATION} DIRECTORY)
    get_filename_component(SGE_CURRENT_AUTOMATION_NAME ${SGE_CURRENT_AUTOMATION_DIR} NAME)
    message("Running '${SGE_CURRENT_AUTOMATION_NAME}' automation...")
    include(${SGE_AUTOMATION})
endforeach()


###########################################
# Include Directories

set(SGE_PUBLIC_INCLUDE_DIRECTORIES
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    "${CMAKE_CURRENT_SOURCE_DIR}/backends/core/interfaces"
)
set(SGE_PRIVATE_INCLUDE_DIRECTORIES)
set(SGE_PUBLIC_SYSTEM_INCLUDE_DIRECTORIES)
set(SGE_PRIVATE_SYSTEM_INCLUDE_DIRECTORIES)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    #list(APPEND SGE_INCLUDE_DIRECTORY)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    #list(APPEND SGE_INCLUDE_DIRECTORY)
endif()


###########################################
# Source Files

set(SGE_DATATYPE_SOURCE_FILES
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

set(SGE_RENDERING_SOURCE_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/rendering/camera.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/rendering/renderer.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/rendering/window.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/rendering/material.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/rendering/mesh.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/rendering/texture.cpp"
#   "${CMAKE_CURRENT_SOURCE_DIR}/src/rendering/shader.cpp"
)

set (SGE_MATH_SOURCE_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/maths/interpolation.cpp"
)

set (SGE_UTILITY_SOURCE_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/utility/imgui_ext.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/utility/raycast.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/utility/spherecast.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/utility/streaming.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/utility/serialization.cpp"

    "${CMAKE_CURRENT_SOURCE_DIR}/src/utility/filesystem/filesystem.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/utility/filesystem/filestream.cpp"
)

set(SGE_SOURCE_FILES
    ${SGE_DATATYPE_SOURCE_FILES}
    ${SGE_RENDERING_SOURCE_FILES}
    ${SGE_MATH_SOURCE_FILES}
    ${SGE_UTILITY_SOURCE_FILES}
 "src/logging.cpp" "src/runtime.cpp")

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    #list(APPEND SOURCE_FILES)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    #list(APPEND SOURCE_FILES)
endif()


###########################################
# Package Dependencies
set(FETCHCONTENT_QUIET OFF)

# Box2D
FetchContent_Declare(Box2D
    GIT_REPOSITORY https://github.com/erincatto/box2d.git
    GIT_TAG v3.0.0
    GIT_SHALLOW ON
    GIT_PROGRESS ON
    EXCLUDE_FROM_ALL)
FetchContent_MakeAvailable(Box2D)
list(APPEND SGE_PUBLIC_SYSTEM_INCLUDE_DIRECTORIES ${box2d_SOURCE_DIR}/include)

# IMPORTANT:
# EABase/EASTL are kept as explicit include dirs here on purpose.
# Recursive submodule fetching and transitive target wiring caused build failures on Windows.
# Do not simplify this without verifying a clean configure/build from scratch.

# EABase
FetchContent_Declare(EABase
    GIT_REPOSITORY https://github.com/electronicarts/EABase.git
    GIT_TAG 0699a15efdfd20b6cecf02153bfa5663decb653c
    GIT_SHALLOW ON
    GIT_PROGRESS ON
    GIT_SUBMODULES ""
    GIT_SUBMODULES_RECURSE OFF
    EXCLUDE_FROM_ALL
)
FetchContent_MakeAvailable(EABase)
list(APPEND SGE_PUBLIC_SYSTEM_INCLUDE_DIRECTORIES ${eabase_SOURCE_DIR}/include/Common)

# EASTL
FetchContent_Declare(EASTL
    GIT_REPOSITORY https://github.com/electronicarts/EASTL.git
    GIT_TAG 3.27.01
    GIT_SHALLOW ON
    GIT_PROGRESS ON
    GIT_SUBMODULES ""
    GIT_SUBMODULES_RECURSE OFF
    EXCLUDE_FROM_ALL
)
FetchContent_MakeAvailable(EASTL)
list(APPEND SGE_PUBLIC_SYSTEM_INCLUDE_DIRECTORIES ${eastl_SOURCE_DIR}/include)

#Dear ImGui
FetchContent_Declare(ImGui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG v1.91.4
    GIT_SHALLOW ON
    GIT_PROGRESS ON
    EXCLUDE_FROM_ALL)
FetchContent_MakeAvailable(ImGui)
list(APPEND SGE_PUBLIC_SYSTEM_INCLUDE_DIRECTORIES ${imgui_SOURCE_DIR})

add_library(ImGui STATIC
    "${imgui_SOURCE_DIR}/imgui.cpp"
    "${imgui_SOURCE_DIR}/imgui_draw.cpp"
    "${imgui_SOURCE_DIR}/imgui_tables.cpp"
    "${imgui_SOURCE_DIR}/imgui_widgets.cpp"
)
target_include_directories(ImGui SYSTEM PUBLIC "${imgui_SOURCE_DIR}")


###########################################
# Defines

set(SGE_PRIVATE_DEFINES "NOMINMAX")
set(SGE_PUBLIC_DEFINES "")

# DEBUG Config
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    #list(APPEND SGE_PRIVATE_DEFINES)
    list(APPEND SGE_PUBLIC_DEFINES
        DEBUG_BUILD
        SGE_ASSERTS_ENABLED
        SGE_IMGUI_IS_ENABLED
    )
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    #list(APPEND SGE_PRIVATE_DEFINES)
    list(APPEND SGE_PUBLIC_DEFINES
        RELEASE_BUILD
        SGE_ASSERTS_ENABLED
    )
endif()

include(TestBigEndian)
TEST_BIG_ENDIAN(IsBigEndianResult)
if(IsBigEndianResult)
    list(APPEND SGE_PUBLIC_DEFINES IS_BIG_ENDIAN)
endif()


############################################
# Finalization

add_library(sge STATIC ${SGE_SOURCE_FILES})
set_target_properties(sge PROPERTIES
  ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
)

target_include_directories(sge
    PUBLIC  ${SGE_PUBLIC_INCLUDE_DIRECTORIES}
    PRIVATE ${SGE_PRIVATE_INCLUDE_DIRECTORIES}
)
target_include_directories(sge
    SYSTEM PUBLIC  ${SGE_PUBLIC_SYSTEM_INCLUDE_DIRECTORIES}
    SYSTEM PRIVATE ${SGE_PRIVATE_SYSTEM_INCLUDE_DIRECTORIES}
)

target_link_libraries(sge
    PUBLIC
        box2d
        EASTL
        ImGui
)

target_compile_definitions(sge
    PUBLIC  ${SGE_PUBLIC_DEFINES}
    PRIVATE ${SGE_PRIVATE_DEFINES}
)

if(MSVC)
    target_compile_options(sge
        PRIVATE /external:W0)
endif()



###########################################
# Platform selection

if(WIN32)
    message("============= Windows platform detected =============")
    set(SGE_DEFAULT_BACKEND "DX12")
elseif(UNIX)
    message("============= Unix platform detected =============")
    set(SGE_DEFAULT_BACKEND "Vulkan")
elseif(APPLE)
    message("============= Apple platform detected =============")
    set(SGE_DEFAULT_BACKEND "Metal")
else()
    message("============= Unable to determine default platform =============")
    set(SGE_DEFAULT_BACKEND "")
endif()

set(SGE_RENDERER_BACKEND
    ${SGE_DEFAULT_BACKEND}
    CACHE STRING "Select the renderer backend to use. Options: DX12, Vulkan, OpenGL, Metal"
)

set_property(CACHE SGE_RENDERER_BACKEND PROPERTY STRINGS DX12 Vulkan OpenGL Metal None)

add_subdirectory(backends/win32)