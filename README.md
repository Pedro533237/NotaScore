# NotaScore

NotaScore é um editor de notação musical em **C++20** focado em:

- alto desempenho em CPU
- baixo consumo de memória
- compatibilidade com hardware antigo (Intel HD integrada)

## Interface profissional (estilo moderno Windows 11)

A interface atual foi estruturada para ser leve e usável:

- janela nativa real em **Windows (Win32/GDI)** e **Linux/X11**
- layout clean com:
  - Top Bar: `Arquivo`, `Editar`, `Preferencias`, `Ajuda`
  - painel lateral Home
  - cartão principal **Nova Partitura**
  - ações: `Abrir Projeto`, `Importar MIDI`, `Importar MusicXML`
  - seção de projetos recentes
- visual leve inspirado em Windows 11:
  - fundo claro `#F3F3F3`
  - painéis brancos
  - destaque azul `#2563EB`
  - bordas suaves/rounded no backend Win32

## Fluxo de criação de partitura

Clique em **Nova Partitura** para abrir assistente lateral leve (sem abrir nova janela pesada):

1. **Seleção de Instrumentos**
   - biblioteca com botões `+`
   - lista de instrumentos selecionados com remoção (`x`)
2. **Configurações da Partitura**
   - armadura
   - compasso
   - andamento (BPM)
   - título
   - compositor

### Modo Performance / Compatibilidade

No assistente:

- toggle `Modo Compatibilidade`
  - desativa animações/sombras/smooth zoom
- toggle `Preview ao vivo`
  - permite desligar preview em máquinas fracas

## Build local

### Linux

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j
ctest --test-dir build --output-on-failure
./build/NotaScore
```

### Windows (MSVC)

```powershell
cmake -S . -B build -G "Visual Studio 17 2022" -A x64
cmake --build build --config Release
ctest --test-dir build -C Release --output-on-failure
```

Artefato alvo: `NotaScore.exe` (GUI subsystem com `WinMain`).

## CI / Artefatos

Workflow: `.github/workflows/build.yml`

- Linux: upload de `NotaScore` e `AppDir`
- Windows: upload de `NotaScore.exe`

## AppImage local

```bash
./packaging/linux/build_appimage.sh
```

- gera `NotaScore-x86_64.AppImage` quando `appimagetool` está presente
- caso contrário, gera `AppDir/` pronto para empacotar
