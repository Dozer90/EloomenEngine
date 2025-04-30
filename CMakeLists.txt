cmake_minimum_required(VERSION 3.28)
project(EloomenateEngine LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

include(FetchContent)

message("============= Active config: ${CMAKE_BUILD_TYPE} =============")

###########################################
# Include Directories

set(INCLUDE_DIRECTORIES
    "include"
)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    #list(APPEND INCLUDE_DIRECTORIES)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    #list(APPEND INCLUDE_DIRECTORIES)
endif()

include_directories(${INCLUDE_DIRECTORIES})


###########################################
# Source Files

set(SOURCES
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

    "src/maths/interpolation.cpp"

    "src/rendering/camera.cpp"
    "src/rendering/window.cpp"

    "src/utility/imgui_ext.cpp"
    "src/utility/raycast.cpp"
    "src/utility/spherecast.cpp"

    "src/main.cpp"
    "src/Engine.cpp"
    "src/TankBot.cpp"
    "src/GameObject.cpp"
    "src/EventSystem.cpp"
    "src/PhysicsSystem.cpp"
    "src/StateMachine.cpp"
    "src/Utilities.cpp"
    "src/Logging.cpp"
    "src/Game.cpp"
)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    #list(APPEND SOURCES)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    #list(APPEND SOURCES)
endif()

add_executable(EloomenateEngine ${SOURCES})

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
target_link_libraries(EloomenateEngine PRIVATE Box2D)
target_include_directories(EloomenateEngine PRIVATE ${box2d_SOURCE_DIR}/include)
target_link_libraries(EloomenateEngine PRIVATE sfml-graphics sfml-window sfml-system sfml-audio)
target_link_libraries(EloomenateEngine PRIVATE EASTL)
target_link_libraries(EloomenateEngine PRIVATE ImGui)
target_include_directories(EloomenateEngine PRIVATE ${imgui_SOURCE_DIR})


###########################################
# Defines

set(PRIVATE_DEFINES "NOMINMAX")
set(PUBLIC_DEFINES "")

# DEBUG Config
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    #list(APPEND PRIVATE_DEFINES)
    list(APPEND PUBLIC_DEFINES
        DEBUG_BUILD
        ASSERTS_ENABLED
        IMGUI_IS_ENABLED
    )
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    #list(APPEND PRIVATE_DEFINES)
    list(APPEND PUBLIC_DEFINES
        RELEASE_BUILD
        ASSERTS_ENABLED
    )
endif()

include(TestBigEndian)
TEST_BIG_ENDIAN(IsBigEndianResult)
if(IsBigEndianResult)
    list(APPEND PUBLIC_DEFINES IS_BIG_ENDIAN)
endif()

message("============= Private compile definitions: ${PRIVATE_DEFINES} =============")
target_compile_definitions(EloomenateEngine PRIVATE ${PRIVATE_DEFINES})

message("============= Public compile definitions: ${PUBLIC_DEFINES} =============")
target_compile_definitions(EloomenateEngine PUBLIC ${PUBLIC_DEFINES})


###########################################
# Features

target_compile_features(EloomenateEngine PRIVATE cxx_std_20)


if(WIN32)
    add_custom_command(
        TARGET EloomenateEngine
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:EloomenateEngine>
        VERBATIM)
endif()