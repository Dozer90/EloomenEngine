# cmake_minimum_required(VERSION 3.28)
# project(ShaderGen LANGUAGES CXX)
# 
# set(CMAKE_CXX_STANDARD 20)
# set(CMAKE_CXX_EXTENSIONS OFF)
# 
# # — fetch SPIRV-Reflect (and any other libs you need) —
# include(FetchContent)
# FetchContent_Declare(SPIRV-Reflect
#     GIT_REPOSITORY https://github.com/KhronosGroup/SPIRV-Reflect.git
#     GIT_TAG        master
# )
# FetchContent_MakeAvailable(SPIRV-Reflect)
# 
# # — build the generator executable —
# add_executable(ShaderGen
#     main.cpp
#     reflection_generator.cpp
# )
# 
# target_include_directories(ShaderGen PRIVATE
#     ${SPIRV-Reflect_SOURCE_DIR}/include
# )
# 
# target_link_libraries(ShaderGen PRIVATE
#     SPIRV-Reflect
# )