# NotaScore

NotaScore é um editor de notação musical em **C++20**, desenhado para funcionar em hardware antigo com foco em execução por CPU e baixo consumo de RAM.

## O que já funciona agora

- App desktop real com janela nativa:
  - **Windows (Win32/GDI)**: abre como app gráfico (sem console).
  - **Linux/X11**: abre janela nativa.
- Tela inicial no estilo dashboard (top bar, sidebar, área de scores, busca e barra inferior).
- Card **New score** clicável: ao clicar, cria uma nova partitura no estado da UI e atualiza o status.
- Núcleo de performance:
  - Thread pool e scheduler por prioridade.
  - Perfil de hardware para modo CPU-first.
  - Base de render incremental/dirty regions.
  - Formato inicial `.nsx` (`NSX1`).

## Build local

### Linux

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j
ctest --test-dir build --output-on-failure
./build/NotaScore
```

> No Linux com ambiente gráfico/X11, o app abre uma janela nativa da interface.

### Windows (MSVC)

```powershell
cmake -S . -B build -G "Visual Studio 17 2022" -A x64
cmake --build build --config Release
ctest --test-dir build -C Release --output-on-failure
```

Artefato alvo: `NotaScore.exe` (subsystem GUI, com `WinMain` dedicado).

## CI / Artefatos (.exe + pacote Linux)

Workflow: `.github/workflows/build.yml`

- Linux: build + testes + upload de `NotaScore` e `AppDir`.
- Windows: build + testes + upload de `NotaScore.exe`.

## AppImage local

```bash
./packaging/linux/build_appimage.sh
```

- Se `appimagetool` existir, gera `NotaScore-x86_64.AppImage`.
- Se não existir, deixa `AppDir/` pronto para você empacotar.
