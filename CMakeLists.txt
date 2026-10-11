cmake_minimum_required(VERSION 3.28)
project(sge LANGUAGES CXX)

set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(engine)
#add_subdirectory(tools/shader_generator)
add_subdirectory(demo_project)

option(SGE_BUILD_TESTS "Build engine regression checks" OFF)
if(SGE_BUILD_TESTS)
    enable_testing()
    add_executable(SGERegressionTests tests/build_regressions.cpp)
    target_link_libraries(SGERegressionTests PRIVATE sge)
    add_test(NAME engine-regressions COMMAND SGERegressionTests)
endif()
