# NotaScore

NotaScore é uma base profissional em **C++20** para um editor de notação musical orientado a:

- CPU-first rendering (funciona sem GPU dedicada)
- Compatibilidade com hardware antigo (Intel i3 1ª geração, 4 GB RAM)
- Arquitetura modular com baixo acoplamento
- Foco em estabilidade, baixo uso de memória e evolução incremental

## Estado atual do projeto

Este commit entrega a fundação técnica para evoluir o produto completo:

- **Core**
  - Thread pool leve
  - Agendador com prioridade (Realtime / Interactive / Background)
  - Heurística automática de perfil de hardware
  - Preparação para `SIMD` via flags de build
- **Render Engine (CPU-first)**
  - Motor de renderização por CPU com dirty regions
  - Renderização incremental (somente regiões marcadas)
- **Motor de notação**
  - Estrutura para notas e reflow lazy (`dirty flag`)
  - Gancho para layout estilo TeX e colisão inteligente
- **Motor de áudio**
  - Configuração de buffer
  - `Playback Lite` para hardware fraco
  - Base para host VST3/LV2 em processo isolado
- **Formato `.nsx`**
  - Estrutura binária inicial (`NSX1`)
  - Persistência e leitura com validação de assinatura
- **Configurações de performance**
  - `CPU Mode Only`
  - `GPU Acceleration (optional)`
  - `Audio buffer size`
  - `Disable animations / shadows / smooth zoom`
  - `Low Memory Mode`

## Build

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

Executável alvo: `NotaScore.exe`.

## Roadmap de produto

1. UI desktop (Qt Widgets ou Dear ImGui) com fallback automático CPU/GPU.
2. Renderização vetorial SMuFL com cache de glyphs em atlas por CPU.
3. Layout avançado (armaduras, quiálteras, polimetria, dinâmica/hairpins, percussão custom).
4. Áudio profissional:
   - VST3 no Windows
   - LV2 no Linux
   - SF2/SFZ com engine leve
5. Formato `.nsx` com salvamento incremental, compactação opcional e crash recovery.
6. Empacotamento:
   - Windows Installer (NSIS/Inno)
   - Linux AppImage e `.deb` opcional.
