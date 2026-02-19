# 🏗️ NotaScore Interface - Build & Deploy Guide

## 📋 Pré-requisitos

### Sistema Operacional
- Linux (Ubuntu 20.04+, Debian 11+)
- Windows 10/11 (com Visual Studio 2019+)
- macOS 10.15+

### Requisitos de Desenvolvimento
- **CMake** 3.20+
- **C++20** compiler (GCC 11+, MSVC 2019+, Clang 13+)
- **Qt6** (Core, Gui, Widgets, OpenGLWidgets)
- **Git** (para controle de versão)

### Hardware Recomendado
- CPU: 2-core 2.0GHz+ (testing em Intel Gen4+)
- RAM: 2GB+ (para compilação, 512MB+ para runtime)
- Disco: 2GB+ (build artifacts)

---

## 🐧 Linux (Ubuntu/Debian)

### 1. Instalar Dependências

```bash
# Update package list
sudo apt-get update

# Instalar Qt6
sudo apt-get install -y \
  qt6-base-dev \
  libqt6opengl6-dev \
  qt6-tools-dev \
  qt6-l10n-tools

# Instalar compiler e ferramentas
sudo apt-get install -y \
  build-essential \
  cmake \
  git \
  gdb \
  valgrind

# (Opcional) Instalar WebEngine para SVG rendering avançado
sudo apt-get install -y qt6-webengine-dev
```

### 2. Clonar Repositório

```bash
git clone https://github.com/Pedro533237/NotaScore.git
cd NotaScore
```

### 3. Compilar com Qt

```bash
# Criar diretório de build
mkdir -p build
cd build

# Configurar CMake com Qt ativado
cmake -S .. -B . \
  -DNOTASCORE_ENABLE_QT=ON \
  -DCMAKE_BUILD_TYPE=Release

# Compilar (usar -j para paralelismo)
cmake --build . -j$(nproc)

# Verificar se compilou
ls -lh NotaScore
```

### 4. Executar

```bash
# Modo desenvolvimento
./NotaScore

# Com debug output
./NotaScore --debug

# Modo compatibilidade (forçar)
QT_QPA_PLATFORMTHEME=gtk3 ./NotaScore
```

### 5. (Opcional) Criar AppImage

```bash
# Build AppImage portável
cd packaging/linux
./build_appimage.sh

# Resultado
ls -lh ../*.AppImage
```

---

## 🪟 Windows (Visual Studio)

### 1. Instalar Qt

1. Download: https://www.qt.io/download-open-source
2. Instalar Qt 6.x (MSVC 2022 64-bit)
3. Anottar caminho de instalação (ex: `C:\Qt\6.7.0\msvc_2022_64`)

### 2. Instalar Ferramentas

```powershell
# Visual Studio 2022 Community (free)
# Download: https://visualstudio.microsoft.com/vs/community/

# CMake
choco install cmake
# ou: https://cmake.org/download/

# Git
choco install git
# ou: https://git-scm.com/download/win
```

### 3. Clonar Repositório

```powershell
git clone https://github.com/Pedro533237/NotaScore.git
cd NotaScore
```

### 4. Compilar

```powershell
# Criar build directory
mkdir build
cd build

# Configure (substituir Qt path)
cmake -S .. -B . `
  -G "Visual Studio 17 2022" -A x64 `
  -DNOTASCORE_ENABLE_QT=ON `
  -DQt6_DIR="C:\Qt\6.7.0\msvc_2022_64\lib\cmake\Qt6" `
  -DCMAKE_BUILD_TYPE=Release

# Build
cmake --build . --config Release -j %NUMBER_OF_PROCESSORS%

# Verificar
dir Release\NotaScore.exe
```

### 5. Executar

```powershell
# Antes de rodar, set Qt DLLs path
set PATH=C:\Qt\6.7.0\msvc_2022_64\bin;%PATH%

# Executar
.\Release\NotaScore.exe
```

### 6. Empacotar (opcional)

```powershell
# Usar windeployqt para copiar DLLs
C:\Qt\6.7.0\msvc_2022_64\bin\windeployqt.exe Release\NotaScore.exe

# Criar ZIP
Compress-Archive -Path Release -DestinationPath NotaScore-Release.zip
```

---

## 🍎 macOS

### 1. Instalar Dependências

```bash
# Usando Homebrew
brew install cmake qt@6

# Ou download manualmente: https://www.qt.io/download
```

### 2. Clonar e Compilar

```bash
git clone https://github.com/Pedro533237/NotaScore.git
cd NotaScore

mkdir build && cd build

cmake -S .. -B . \
  -DNOTASCORE_ENABLE_QT=ON \
  -DCMAKE_BUILD_TYPE=Release

cmake --build . -j $(sysctl -n hw.ncpu)
```

### 3. Executar

```bash
./NotaScore
```

### 4. Criar Bundle (para distribuição)

```bash
# Copiar Qt frameworks
macdeployqt NotaScore.app

# Assinar (opcional, para Gatekeeper)
codesign -s - NotaScore.app

# Criar DMG
hdiutil create -volname NotaScore -srcfolder . -ov -format UDZO NotaScore.dmg
```

---

## 🔍 Build Sem Qt (Nativo)

Se Qt não estiver disponível:

```bash
# Linux/macOS
cmake -S . -B build -DNOTASCORE_ENABLE_QT=OFF
cmake --build build

# Windows (Visual Studio)
cmake -S . -B build -G "Visual Studio 17 2022" -A x64 -DNOTASCORE_ENABLE_QT=OFF
cmake --build build --config Release
```

---

## 🧪 Validação Pós-Build

### 1. Executar Smoke Tests

```bash
# Teste básico
./build/notascore_smoke
# Esperado: Todos os testes passarem

# Ou com ctest
cd build
ctest
```

### 2. Verificar Performance

```bash
# Memory usage
/usr/bin/time -v ./NotaScore

# Esperado: ~100-200MB peak

# CPU profiling
perf record ./NotaScore
perf report
```

### 3. Teste Visual

- [ ] Interface aparece com tema claro
- [ ] Menu bar visível (Arquivo, Editar, Ajuda)
- [ ] Cartão "Nova Partitura" renderiza corretamente
- [ ] Painel lado direito mostra wizard
- [ ] Botões respondem ao hover
- [ ] Nenhuma erro/warning no console

---

## 🐛 Troubleshooting

### CMake não encuentra Qt6

```bash
# Solução 1: Especificar manualmente
cmake -DQt6_DIR=/path/to/Qt6/lib/cmake/Qt6 ...

# Solução 2: Usar find_package()
# Em CMakeLists.txt, adicionar:
list(APPEND CMAKE_PREFIX_PATH "/usr/lib/cmake/Qt6")
```

### Erro: "No such file or directory: <QMainWindow>"

```bash
# Qt não está instalado ou path incorreto
# Linux:
sudo apt-get install qt6-base-dev

# macOS:
brew install qt@6

# Windows:
# Reinstalar Qt via Qt Online Installer
```

### Aplicação lenta / 30fps

```bash
# Ativar modo compatibilidade:
./NotaScore --compatibility-mode

# Ou:
export NOTASCORE_COMPAT_MODE=1
./NotaScore
```

### Memory leak detectado

```bash
# Usar Valgrind (Linux)
valgrind --leak-check=full --show-leak-kinds=all ./NotaScore

# Usar Xcode Instruments (macOS)
# Cmd+I em Xcode

# Usar VLD (Windows)
# https://github.com/KindDragon/vld
```

---

## 📦 Deployment

### Linux AppImage

```bash
cd packaging/linux
./build_appimage.sh

# Resultado: NotaScore-x86_64.AppImage
# Chamar como: ./NotaScore-x86_64.AppImage
```

### Windows MSI Installer (opcional)

```bash
# Usar WiX Toolset
# https://wixtoolset.org/

# Ou NSIS:
# https://nsis.sourceforge.io/

# Template:
# - Incluir NotaScore.exe
# - Copiar Qt DLLs (via windeployqt)
# - Set registry para associações
```

### macOS DMG

```bash
# Build bundle
cmake --build build --target bundle

# Criar DMG
hdiutil create -volname "NotaScore" \
  -srcfolder build/NotaScore.app \
  -ov -format UDZO NotaScore.dmg

# Colocar em website para download
```

---

## 🔐 Release Build Checklist

- [ ] `CMAKE_BUILD_TYPE=Release`
- [ ] Nenhum debug symbols (ou stripped)
- [ ] Modo compatibilidade testado
- [ ] Memory footprint <200MB
- [ ] FPS 60+ em Intel HD Gen3
- [ ] Tamanho binário aceitável (<50MB)
- [ ] Dependências Qt empacotadas
- [ ] Atalhos funcionam
- [ ] Tema light/dark funciona
- [ ] Preview ao vivo (se habilitado)
- [ ] Nenhum warning de compilação
- [ ] Documentação atualizada

---

## 🚀 Quick Start Commands

### Linux/macOS
```bash
git clone https://github.com/Pedro533237/NotaScore.git
cd NotaScore
mkdir -p build && cd build
cmake -S .. -B . -DNOTASCORE_ENABLE_QT=ON -DCMAKE_BUILD_TYPE=Release
cmake --build . -j$(nproc)
./NotaScore
```

### Windows (PowerShell)
```powershell
git clone https://github.com/Pedro533237/NotaScore.git
cd NotaScore
mkdir build; cd build
cmake -S .. -B . -G "Visual Studio 17 2022" -A x64 -DNOTASCORE_ENABLE_QT=ON `
  -DQt6_DIR="C:\Qt\6.7.0\msvc_2022_64\lib\cmake\Qt6"
cmake --build . --config Release -j %NUMBER_OF_PROCESSORS%
.\Release\NotaScore.exe
```

---

## 📊 Build Times (Referência)

| Hardware | Tempo |
|----------|-------|
| Intel i5-10400 (6c/12t) | ~45s |
| Intel i7-11700 (8c/16t) | ~30s |
| Apple M1 | ~25s |
| Intel HD Gen4 (varia) | ~90s |

---

## 🔗 Recursos Úteis

- **Qt Documentation**: https://doc.qt.io/qt-6/
- **CMake Guide**: https://cmake.org/cmake/help/latest/
- **NotaScore Repo**: https://github.com/Pedro533237/NotaScore
- **Report Issues**: https://github.com/Pedro533237/NotaScore/issues

---

**Última atualização**: 2026-02-19  
**Versão**: 0.1.0
