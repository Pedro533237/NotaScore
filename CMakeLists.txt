cmake_minimum_required(VERSION 3.20)
project(NotaScore VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(NOTASCORE_ENABLE_SIMD "Enable SIMD-friendly code paths" ON)
option(NOTASCORE_ENABLE_OPENGL "Enable optional legacy OpenGL backend" OFF)

if(MSVC)
    add_compile_options(/W4 /permissive- /EHsc)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -Wconversion)
endif()

add_library(notascore_core
    src/core/ThreadPool.cpp
    src/core/TaskScheduler.cpp
    src/core/PerformanceProfile.cpp
)

target_include_directories(notascore_core PUBLIC include)

target_compile_definitions(notascore_core PUBLIC
    $<$<BOOL:${NOTASCORE_ENABLE_SIMD}>:NOTASCORE_ENABLE_SIMD=1>
    $<$<BOOL:${NOTASCORE_ENABLE_OPENGL}>:NOTASCORE_ENABLE_OPENGL=1>
)

add_library(notascore_engine
    src/render/CpuRenderer.cpp
    src/notation/NotationEngine.cpp
    src/audio/AudioEngine.cpp
    src/io/NsxDocument.cpp
    src/ui/PerformanceSettings.cpp
    src/app/Application.cpp
)

target_include_directories(notascore_engine PUBLIC include)
target_link_libraries(notascore_engine PUBLIC notascore_core)

add_executable(NotaScore src/main.cpp)
target_link_libraries(NotaScore PRIVATE notascore_engine)

include(CTest)
if(BUILD_TESTING)
    add_executable(notascore_smoke tests/smoke.cpp)
    target_link_libraries(notascore_smoke PRIVATE notascore_engine)
    add_test(NAME smoke COMMAND notascore_smoke)
endif()
