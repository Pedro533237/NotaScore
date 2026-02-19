cmake_minimum_required(VERSION 3.20)
project(NotaScore VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(NOTASCORE_ENABLE_SIMD "Enable SIMD-friendly code paths" ON)
option(NOTASCORE_ENABLE_OPENGL "Enable optional legacy OpenGL backend" OFF)
option(NOTASCORE_ENABLE_QT "Enable optional Qt-based UI" OFF)

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
    src/ui/MainWindow.cpp
    src/ui/Theme.cpp
    src/ui/ModernWidgets.cpp
    src/platform/NativeWindow.cpp
    $<$<BOOL:${NOTASCORE_ENABLE_QT}>:src/platform/NativeWindowQt.cpp>
    $<$<BOOL:${NOTASCORE_ENABLE_QT}>:src/ui/QtMainWindow.cpp>
    $<$<BOOL:${NOTASCORE_ENABLE_QT}>:src/app/NotascoreApplication.cpp>
    src/ui/PreviewWidget.cpp
    src/app/Application.cpp
)

target_include_directories(notascore_engine PUBLIC include)
target_link_libraries(notascore_engine PUBLIC notascore_core)

if(UNIX AND NOT APPLE)
    find_package(X11 REQUIRED)
    target_include_directories(notascore_engine PRIVATE ${X11_INCLUDE_DIR})
    target_link_libraries(notascore_engine PUBLIC ${X11_LIBRARIES})
endif()

if(WIN32)
    add_executable(NotaScore WIN32 src/main.cpp src/platform/winmain.cpp)
else()
    add_executable(NotaScore src/main.cpp)
endif()

target_link_libraries(NotaScore PRIVATE notascore_engine)

if(NOTASCORE_ENABLE_QT)
    find_package(Qt6 REQUIRED COMPONENTS Widgets OpenGLWidgets)
    # WebEngineWidgets is optional (heavy). Try to find it, but continue if missing.
    find_package(Qt6 QUIET COMPONENTS WebEngineWidgets)
    target_link_libraries(notascore_engine PUBLIC Qt6::Widgets Qt6::OpenGLWidgets)
    if(TARGET Qt6::WebEngineWidgets)
        target_link_libraries(notascore_engine PUBLIC Qt6::WebEngineWidgets)
        target_compile_definitions(notascore_engine PUBLIC NOTASCORE_QT_WEBENGINE=1)
    else()
        message(STATUS "Qt6::WebEngineWidgets not found; continuing without WebEngine support.")
    endif()
    target_compile_definitions(notascore_engine PUBLIC NOTASCORE_ENABLE_QT=1)
    target_compile_definitions(NotaScore PUBLIC NOTASCORE_ENABLE_QT=1)
endif()

include(CTest)
if(BUILD_TESTING)
    add_executable(notascore_smoke tests/smoke.cpp)
    target_link_libraries(notascore_smoke PRIVATE notascore_engine)
    add_test(NAME smoke COMMAND notascore_smoke)
endif()
