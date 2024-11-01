cmake_minimum_required(VERSION 3.28)
project(TankWarz LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

include(FetchContent)

# Anything here will be included in all build configs
set(INCLUDE_DIRECTORIES
    "include"
)

set(SOURCES
    "src/main.cpp"
    "src/Game.cpp"
    "src/TankBot.cpp"
    "src/GameObject.cpp"
)

set(DEFINITIONS)

# DEBUG Config
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "Configuring for Debug mode")

    list(APPEND INCLUDE_DIRECTORIES
    )

    list(APPEND SOURCES
    )

    list(APPEND DEFINITIONS
        IMGUI_IS_ENABLED
    )

# RELEASE Config
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    message(STATUS "Configuring for Release mode")

endif()

add_executable(TankWarz ${SOURCES})

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
target_link_libraries(TankWarz PRIVATE Box2D)
target_link_libraries(TankWarz PRIVATE sfml-graphics sfml-window sfml-system sfml-audio)
target_link_libraries(TankWarz PRIVATE EASTL)
target_link_libraries(TankWarz PRIVATE ImGui)

###########################################

target_compile_features(TankWarz PRIVATE cxx_std_17)

target_compile_definitions(TankWarz PUBLIC ENGINE_NAMESPACE=TankWarz)

if(WIN32)
    add_custom_command(
        TARGET TankWarz
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:TankWarz>
        VERBATIM)
endif()