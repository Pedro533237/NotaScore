# NotaScore — Qt Setup Guide

## Quick Start (Without Qt)

The project builds **by default without Qt dependencies**, using native platform backends (Win32 on Windows, X11 on Linux).

```bash
cmake -S . -B build
cmake --build build --config Release
./build/NotaScore
```

## Enabling Qt-based UI

Qt is **optional** and can be activated at configure time.

### Linux (Ubuntu/Debian)

1. **Install Qt6 packages:**
```bash
sudo apt-get update
sudo apt-get install -y qt6-base-dev libqt6opengl6-dev qt6-tools-dev
```

2. **Configure and build with Qt:**
```bash
cmake -S . -B build -DNOTASCORE_ENABLE_QT=ON
cmake --build build -j$(nproc)
./build/NotaScore
```

**Optional:** Install WebEngine for advanced SVG rendering:
```bash
sudo apt-get install -y qt6-webengine-dev
```

### Windows (Visual Studio + Qt Installer)

1. **Install Qt6** via [Qt Online Installer](https://www.qt.io/download-open-source):
   - Download and run installer
   - Select `Qt → 6.x.x → MSVC 2022 64-bit`
   - Note the install path (e.g., `C:\Qt\6.7.0\msvc_2022_64`)

2. **Configure with Qt:**
```bash
cmake -S . -B build -G "Visual Studio 17 2022" -A x64 `
  -DNOTASCORE_ENABLE_QT=ON `
  -DQt6_DIR="C:\Qt\6.7.0\msvc_2022_64\lib\cmake\Qt6"

cmake --build build --config Release
.\build\Release\NotaScore.exe
```

**Replace** `C:\Qt\6.7.0\msvc_2022_64` with your actual Qt install path.

### Windows (vcpkg — Recommended for CI/Automation)

1. **Clone vcpkg:**
```bash
git clone https://github.com/microsoft/vcpkg.git
.\vcpkg\bootstrap-vcpkg.bat
```

2. **Install Qt and dependencies:**
```bash
.\vcpkg\vcpkg.exe install qt6-base:x64-windows qt6-openglwidgets:x64-windows
```

3. **Configure with vcpkg:**
```bash
cmake -S . -B build -G "Visual Studio 17 2022" -A x64 `
  -DNOTASCORE_ENABLE_QT=ON `
  -DCMAKE_TOOLCHAIN_FILE=C:\path\to\vcpkg\scripts\buildsystems\vcpkg.cmake

cmake --build build --config Release
.\build\Release\NotaScore.exe
```

### macOS

1. **Install Qt6 via Homebrew:**
```bash
brew install qt6
```

2. **Configure and build:**
```bash
export Qt6_DIR="$(brew --prefix qt6)/lib/cmake/Qt6"
cmake -S . -B build -DNOTASCORE_ENABLE_QT=ON
cmake --build build -j$(sysctl -n hw.ncpu)
./build/NotaScore
```

## Build Flags

| Flag | Default | Description |
|------|---------|-------------|
| `NOTASCORE_ENABLE_QT` | `OFF` | Enable Qt6 Widgets UI |
| `NOTASCORE_ENABLE_SIMD` | `ON` | Enable SIMD optimizations |
| `NOTASCORE_ENABLE_OPENGL` | `OFF` | Enable legacy OpenGL 3.3 backend |

## Qt Features

When `NOTASCORE_ENABLE_QT=ON`, the app includes:

- **Native Qt Widgets UI** with modern styling (Windows 11-inspired)
- **Instrument Library** with virtualized list view
- **Score Wizard** (2-step: instruments, settings)
- **Live Preview Widget** (QOpenGLWidget-based)
- **Performance Mode** toggle for older GPUs
- **Light/Dark Theme** support (optional)

## Fallback Behavior

If Qt is disabled or WebEngineWidgets is unavailable:

- **Windows**: Uses native Win32 GDI rendering
- **Linux**: Uses X11 with basic Xlib drawing
- **macOS**: Falls back to Cocoa (if implemented)

All platform-specific backends render the same UI model via the CPU renderer.

## Troubleshooting

### CMake can't find Qt6
- **Windows**: Use `-DQt6_DIR` flag (see Windows section above)
- **Linux**: Run `cmake -S . -B build -DNOTASCORE_ENABLE_QT=OFF` to skip Qt
- **macOS**: Ensure Homebrew Qt6 is installed; check `brew --prefix qt6`

### Qt6Config.cmake not found
This is expected if Qt is not installed. Simply build without Qt:
```bash
cmake -S . -B build
cmake --build build
```

### Symbol conflicts or multiple NativeWindow definitions
Run a clean build:
```bash
rm -rf build
cmake -S . -B build -DNOTASCORE_ENABLE_QT=OFF  # or ON, as desired
cmake --build build
```

## Performance Notes

- **Qt UI** is lightweight; no heavy dependencies like QtWebEngine are required.
- With `NOTASCORE_ENABLE_QT=OFF`, the app is ~50MB smaller (no Qt libs).
- Both paths support OpenGL 3.3 render and CPU fallback.
- Memory idle: <200MB target (Qt or native).
