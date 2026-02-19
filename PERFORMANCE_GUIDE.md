# NotaScore - Guia de Otimização de Performance

## 📊 Benchmarks Esperados

| Métrica | Target | Compatível |
|---------|--------|-----------|
| **Memory (Idle)** | <200MB | ✓ |
| **Memory (Score open)** | <400MB | ✓ |
| **UI Framerate** | 60fps | Intel HD Gen4+ |
| **Score Render** | 30fps | Intel HD Gen4+ |
| **GPU Memory** | <512MB | Integrated GPU |
| **Startup Time** | <2s | SSD |

## 🎯 Estratégias de Otimização

### 1. Renderização

#### ✅ Prioridade 1: Renderização Vetorial Leve

```cpp
// Usar QPainter com renderização vetorial
void drawScore(QPainter& painter) {
    // Bom: Rasterização segura
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
    
    // Evitar: Hardware complexo
    // - Gradientes pesados
    // - Blur real-time
    // - Shaders customizados
    
    // Desenhar clefs, notas, linhas de pauta
    drawStaff(painter);
    drawNotes(painter);
}
```

#### ✅ Prioridade 2: Caching de Renderização

```cpp
class CachedScoreView : public QWidget {
private:
    QPixmap m_cache;
    bool m_cacheDirty{true};
    
public:
    void markDirty() { m_cacheDirty = true; }
    
protected:
    void paintEvent(QPaintEvent* event) override {
        if (m_cacheDirty) {
            m_cache = QPixmap(size());
            QPainter cachePainter(&m_cache);
            renderScore(cachePainter);
            m_cacheDirty = false;
        }
        
        QPainter painter(this);
        painter.drawPixmap(0, 0, m_cache);
    }
};
```

#### ✅ Prioridade 3: Disable Antialiasing em modo Performance

```cpp
void renderUI(QPainter& painter, bool performanceMode) {
    if (!performanceMode) {
        painter.setRenderHint(QPainter::Antialiasing);
    }
    
    // Desenhá interface
    drawModernCard(painter);
    drawButtons(painter);
}
```

### 2. Memória

#### ✅ Object Pooling para Widgets

```cpp
class WidgetPool {
    std::vector<ModernButton*> m_buttonPool;
    
public:
    ModernButton* acquire() {
        if (!m_buttonPool.empty()) {
            auto btn = m_buttonPool.back();
            m_buttonPool.pop_back();
            btn->show();
            return btn;
        }
        return new ModernButton();
    }
    
    void release(ModernButton* btn) {
        btn->hide();
        m_buttonPool.push_back(btn);
    }
};
```

#### ✅ Virtualização de Listas

```cpp
class VirtualInstrumentList : public QListWidget {
    static constexpr int VISIBLE_ITEMS = 6;
    std::vector<InstrumentDef> m_data;
    
public:
    void setData(const std::vector<InstrumentDef>& data) {
        m_data = data;
        setMaximumHeight(VISIBLE_ITEMS * 34);  // Altura fixa
        updateVisibleItems();
    }
    
private:
    void updateVisibleItems() {
        clear();
        for (size_t i = 0; i < std::min(VISIBLE_ITEMS, m_data.size()); ++i) {
            addItem(QString::fromStdString(m_data[i].name));
        }
    }
};
```

#### ✅ Lazy-Loading de Recursos

```cpp
class AsyncIconLoader : public QObject {
    Q_OBJECT
    
public:
    void loadIcon(const QString& path) {
        QtConcurrent::run([this, path]() {
            QIcon icon(path);
            emit iconReady(icon);
        });
    }
    
signals:
    void iconReady(const QIcon& icon);
};
```

### 3. CPU

#### ✅ Multithreading para Operações Pesadas

```cpp
class ScoreCompiler {
    void compileAsync(const NotationEngine::Document& doc) {
        QtConcurrent::run([doc]() {
            auto compiled = compileDocument(doc);
            emit compilationFinished(compiled);
        });
    }
};
```

#### ✅ Debouncing para Eventos Frequentes

```cpp
class DebouncedPropertyBinding {
    QTimer m_debounce;
    
public:
    void onPropertyChanged() {
        m_debounce.stop();
        m_debounce.start(250ms);  // Aguardar 250ms de inatividade
    }
    
private slots:
    void onDebounceTimeout() {
        updateProperty();
    }
};
```

#### ✅ Processamento em Chunks

```cpp
void renderLargeScore(const Note notes[], int count) {
    constexpr int CHUNK_SIZE = 100;
    
    for (int i = 0; i < count; i += CHUNK_SIZE) {
        int end = std::min(i + CHUNK_SIZE, count);
        // Processar chunk
        for (int j = i; j < end; ++j) {
            renderNote(notes[j]);
        }
        // Permitir UI responder
        QApplication::processEvents();
    }
}
```

### 4. GPU (Integrated Graphics)

#### ✅ Requisitos Mínimos

- Intel HD Graphics 3000+ (2011+)
- AMD Radeon Integrated R7+
- NVIDIA GTX 750M+
- OpenGL 3.3+ ou Direct2D

#### ✅ Evitar

❌ Blur real-time (Box Blur, Gaussian Blur)  
❌ Sombras dinâmicas complexas  
❌ Gradientes radiais/cônicos  
❌ Efeitos de partículas  
❌ Múltiplos render targets  

#### ✅ Alternativas

- **Blur**: Blur fake estático (pre-rendered)
- **Sombras**: Box shadows simples (apenas offset + opacidade)
- **Gradientes**: Linear simples apenas
- **Efeitos**: Desativar em modo compatibilidade

### 5. Modo Compatibilidade Completo

| Feature | Normal | Compatível |
|---------|--------|-----------|
| Animações | ✓ | ✗ |
| Sombras | ✓ | ✗ |
| Zoom suave | ✓ | ✗ |
| Preview ao vivo | ✓ | ✗ |
| Antialiasing | ✓ | ✗ |
| Renderização Score | GPU | CPU |

```cpp
void applyCompatibilityMode() {
    g_settings.disableAnimations = true;
    g_settings.disableShadows = true;
    g_settings.disableSmoothZoom = true;
    g_settings.useSimplifiedRendering = true;
    
    // Desativar preview em tempo real
    scorePreview->setLiveRenderingEnabled(false);
    
    // Usar renderização CPU
    scorePreview->setRenderMode(RenderMode::CPU);
}
```

## 📈 Profiling

### Com Qt Creator

```bash
# Build com debug symbols
cmake -S . -B build -DNOTASCORE_ENABLE_QT=ON -DCMAKE_BUILD_TYPE=RelWithDebInfo

# Profile
qmlprofiler /path/to/NotaScore  # QML Profiler
# ou use Firefox Profiler
perf record ./NotaScore
perf report
```

### Checklist de Performance

- [ ] Memory footprint < 200MB idle
- [ ] 60fps UI em Intel HD Gen4
- [ ] Startup < 2 segundos
- [ ] Score render 30fps mínimo
- [ ] Compatibilidade mode detectada automaticamente
- [ ] Zero vazamento de memória
- [ ] Nenhuma thread bloqueada
- [ ] CPU < 10% em idle

## 🔍 Análise de Gargalos

### Se UI é lenta:

1. **Verificar FPS:**
   ```cpp
   QTimer fpsTimer;
   int frameCount = 0;
   connect(&fpsTimer, &QTimer::timeout, [&]() {
       qDebug() << "FPS:" << frameCount;
       frameCount = 0;
   });
   fpsTimer.start(1000);
   ```

2. **Identificar widget problemático:**
   ```cpp
   // Desabilitar widgets até encontrar o culpado
   modernCard->setVisible(false);  // Teste
   ```

3. **Analisar pintura:**
   ```cpp
   // Em ModernWidgets.cpp
   void ModernButton::paintEvent(QPaintEvent* event) {
       auto t0 = std::chrono::high_resolution_clock::now();
       // ... paint code ...
       auto t1 = std::chrono::high_resolution_clock::now();
       qDebug() << "Paint took:" 
                << std::chrono::duration_cast<std::chrono::microseconds>(t1-t0).count() 
                << "μs";
   }
   ```

### Se memória é alta:

1. **Usar valgrind:**
   ```bash
   valgrind --leak-check=full ./NotaScore 2>&1 | tee memprofile.txt
   ```

2. **Verificar alocações:**
   ```cpp
   // Em cada aloc crítico
   Q_ASSERT_X(m_data.size() < 1000000, "loadData", "Too much memory");
   ```

3. **Usar Qt Memory Profiler:**
   ```bash
   # Build com -fno-omit-frame-pointer
   cmake ... -DCMAKE_CXX_FLAGS="-fno-omit-frame-pointer"
   ```

## 🚀 Deployment para PCs Fracos

### Windows (Intel HD Graphics)

```batch
REM Build Release otimizado
cmake -G "Visual Studio 17 2022" -A x64 ^
  -DNOTASCORE_ENABLE_QT=ON ^
  -DCMAKE_BUILD_TYPE=Release

REM Strips debug symbols
strip NotaScore.exe

REM Size: ~30-40MB
```

### Linux (AppImage)

```bash
# Cria AppImage portável
./packaging/linux/build_appimage.sh

# Teste em VM com Intel HD Gen3
qemu-system-x86_64 -enable-kvm \
  -m 2G \
  -drive file=ubuntu-22.04.iso,format=iso \
  -device intel-iommu \
  -device intel-hda
```

## 📋 Checklist Pré-Release

- [ ] Modo Compatibilidade funciona
- [ ] Memory profiling < 200MB
- [ ] FPS 60+ em Intel HD
- [ ] Teste em Notebook com 2GB RAM
- [ ] Sem vazamento de memória (valgrind)
- [ ] Startup < 2s
- [ ] AppImage < 50MB
- [ ] Todos cores arredondados renderizando corretamente
- [ ] Shadows invisíveis em modo compatível
- [ ] Sombra de texto legível em tema escuro

---

**Última atualização**: 2026-02-19
