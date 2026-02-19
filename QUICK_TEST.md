# NotaScore — Quick Test Guide

## Test the Build (No Qt Required)

```bash
# Clean build without Qt dependencies
rm -rf build
cmake -S . -B build
cmake --build build -j$(nproc)

# Run the default (native UI)
./build/NotaScore
```

**Expected**: App launches with native X11 (Linux) or Win32 (Windows) UI.

---

## Test with Qt (Linux Only, This Container)

If you have Qt6 installed on this container:

```bash
# Build with Qt enabled
rm -rf build
cmake -S . -B build -DNOTASCORE_ENABLE_QT=ON
cmake --build build -j$(nproc)

# Run the Qt UI
./build/NotaScore
```

**Expected**: App launches with Qt Widgets UI (prettier, modern styling).

---

## Windows Instructions (For Your Local Machine)

See [QT_SETUP.md](QT_SETUP.md) for detailed setup.

**TL;DR** — Two options:

### Option A: No Qt (Fastest)
```bash
cmake -S . -B build -G "Visual Studio 17 2022" -A x64
cmake --build build --config Release
.\build\Release\NotaScore.exe
```

### Option B: With Qt (Best UI)
Install Qt, then:
```bash
cmake -S . -B build -G "Visual Studio 17 2022" -A x64 `
  -DNOTASCORE_ENABLE_QT=ON `
  -DQt6_DIR="C:\Qt\6.7.0\msvc_2022_64\lib\cmake\Qt6"
cmake --build build --config Release
.\build\Release\NotaScore.exe
```

---

## macOS Instructions

See [QT_SETUP.md](QT_SETUP.md) for Homebrew + CMake.

---

## Troubleshooting

| Issue | Solution |
|-------|----------|
| `cmake` not found | Install CMake: `apt install cmake` (Linux), or download installer (Windows/macOS) |
| X11 not available | You're on macOS or headless Linux; Qt is required for UI |
| Qt6 not found (Windows error) | Add `-DQt6_DIR="..."` flag (see QT_SETUP.md) |
| Build still uses Qt (unwanted) | Use `-DNOTASCORE_ENABLE_QT=OFF` explicitly |
| Symbols conflict | Run `rm -rf build` and reconfigure |

---

## Code Walkthrough

- **Model**: `include/notascore/ui/MainWindow.hpp` — State management
- **Qt UI**: `src/ui/QtMainWindow.cpp` — If Qt enabled
- **Native Backend**: `src/platform/NativeWindow.cpp` — If Qt disabled
- **Build Config**: `CMakeLists.txt` — Conditional compilation

---

**Ready to test?** Start with the no-Qt build above.
