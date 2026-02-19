# NotaScore Interface Documentation

## 📋 Visão Geral

Este documento descreve a arquitetura completa da interface gráfica do NotaScore, um editor profissional de notação musical otimizado para:

- **Alto Desempenho**: Interface fluida em GPUs integradas antigas (Intel HD)
- **Baixo Consumo de Memória**: <200MB em idle
- **Design Moderno**: Inspirado em Windows 11
- **Acessibilidade**: Temas claro/escuro com contraste adequado

## 🏗️ Arquitetura de Componentes

### 1. Sistema de Temas (`Theme.hpp` / `Theme.cpp`)

O sistema de temas é centralizado e permite uma mudança dinâmica entre tema claro e escuro.

**Características:**
- Paleta de cores predefinidas (Light/Dark)
- Métricas de design (espacimento, cantos arredondados, sombras)
- Tipografia padronizada
- Transições animadas opcionais

**Uso:**
```cpp
#include "notascore/ui/Theme.hpp"

// Alterar modo do tema
g_theme.setMode(ThemeMode::Dark);

// Acessar cores
Color bgColor = g_theme.backgroundColor;
Color accentColor = g_theme.accentColor;

// Verificar modo atual
if (g_theme.isDark()) {
    // Executar lógica específica para tema escuro
}
```

**Paleta Light (Windows 11):**
- Fundo principal: #F3F3F3
- Painéis: #FFFFFF
- Destaque/Accent: #2563EB (azul)
- Hover: #E5F0FF
- Texto primário: #1A1A1A
- Dividers: #E0E0E0

**Paleta Dark:**
- Fundo: #1F1F1F
- Painéis: #2A2A2A
- Accent: #3B82F6 (azul mais claro)
- Texto: #F5F5F5
- Dividers: #323232

### 2. Widgets Modernos (`ModernWidgets.hpp` / `ModernWidgets.cpp`)

Conjunto de componentes Qt customizados com renderização sofisticada.

#### 2.1 ModernCard
Cartão com sombra suave e hover interativo.

```cpp
auto* card = new ModernCard;
card->setTitle("Nova Partitura");
card->setSubtitle("Criar uma nova partitura em branco");
card->setIcon(noteIcon);
card->setHoverable(true);

connect(card, &ModernCard::clicked, this, &MyWindow::onNewScore);
```

**Recursos:**
- Cantos arredondados (12px)
- Sombra suave (não usa blur real-time em modo performance)
- Efeito hover com cor overlay
- Ícone customizável

#### 2.2 ModernButton
Botão com três estilos (Primary, Secondary, Subtle).

```cpp
// Estilo Primary (azul, preenchido)
auto* primaryBtn = new ModernButton("Confirmar", ModernButton::Style::Primary);

// Estilo Secondary (outline)
auto* secondaryBtn = new ModernButton("Cancelar", ModernButton::Style::Secondary);

// Estilo Subtle (texto simples)
auto* subtleBtn = new ModernButton("Remover", ModernButton::Style::Subtle);

connect(primaryBtn, &ModernButton::clicked, this, &MyWindow::onConfirm);
```

**Características:**
- Transição suave entre estados (hover, pressed)
- Desativável
- Ícone opcional
- Tamanho adaptável

#### 2.3 ModernLineEdit
Campo de entrada customizado com label.

```cpp
auto* titleInput = new ModernLineEdit;
titleInput->setLabel("Título da Partitura");
titleInput->setPlaceholder("Insira o título...");

connect(titleInput, &ModernLineEdit::textChanged, 
        this, &MyWindow::onTitleChanged);
```

#### 2.4 ModernSlider
Slider customizado para valores numéricos.

```cpp
auto* tempoSlider = new ModernSlider;
tempoSlider->setLabel("Andamento (BPM)");
tempoSlider->setRange(40, 240);
tempoSlider->setValue(120);

connect(tempoSlider, qOverload<int>(&ModernSlider::valueChanged),
        this, &MyWindow::onTempoChanged);
```

#### 2.5 PerformanceToggle
Checkbox customizado com descrição para configurações de performance.

```cpp
auto* compatToggle = new PerformanceToggle("Modo Compatibilidade");
compatToggle->setDescription("Desativa sombras e animações");
compatToggle->setChecked(false);

connect(compatToggle, &PerformanceToggle::toggled,
        this, [this](bool checked) {
    if (checked) {
        g_theme.setMode(ThemeMode::Light);
        disableAnimations();
        disableShadows();
    }
});
```

### 3. Stylesheet Moderno (`styles-modern.qss`)

Arquivo CSS especializado para Qt que implementa o design Windows 11.

**Características:**
- Tema claro e tema escuro completos
- Suporte para todos widgets Qt padrão
- Cantos arredondados suave
- Sombras otimizadas (não há blur em PCs fracos)
- Transições via hover
- Design consistente

**Uso:**
```cpp
// Aplicar stylesheet
QFile file(":/resources/styles-modern.qss");
if (file.open(QFile::ReadOnly)) {
    QString style = QLatin1String(file.readAll());
    qApp->setStyleSheet(style);
}

// Alternar tema dinamicamente
void setTheme(bool isDark) {
    qApp->setProperty("theme", isDark ? "dark" : "light");
    // Stylesheet será reaplicado automaticamente
}
```

## 🎯 Fluxo da Interface - Home Screen

A interface segue um layout moderno em duas colunas:

```
┌─────────────────────────────────────────────────────────────────┬─────────────┐
│                    MENU BAR (Arquivo, Editar, Ajuda)            │             │
├─────────────────────────────────────────────────────────────────┼─────────────┤
│                                                                  │             │
│  HOME - Logo & Título                                           │   WIZARD    │
│  ========================================                        │   PAINEL    │
│  🎼 NotaScore                                                   │             │
│                                                                  │   Step 1:   │
│  ┌──────────────────────────────────────────┐                   │  Select     │
│  │   Nova Partitura (Card com hover)        │                   │  Instrs     │
│  │                                          │                   │             │
│  │     🎼  Novo empty score                 │                   │  ┌────────┐ │
│  └──────────────────────────────────────────┘                   │  │ + Inst │ │
│                                                                  │  │ + Inst │ │
│  [Abrir] [Importar MIDI] [Importar MusicXML]                    │  └────────┘ │
│                                                                  │             │
│  Projetos Recentes                                              │  Selecionad │
│  ┌────────────────────────────────────────────┐                │  │ Inst 1  │ │
│  │ • String Quartet in D                      │                │  │ Inst 2  │ │
│  │ • Film Cue Sketch                          │                │  └────────┘ │
│  │ • Piano Etude No. 2                        │                │             │
│  └────────────────────────────────────────────┘                │  ☑ Compat   │
│                                                                  │  ☑ Preview  │
│                                                                  │             │
│                                                                  │ [Back][Next]│
└─────────────────────────────────────────────────────────────────┴─────────────┘
```

## 🧙 Assistente de Criação de Partitura (Wizard)

O wizard é um painel lateral que guia o usuário através de 2 passos:

### Passo 1: Seleção de Instrumentos

**Layout:**
```
┌─────────────────────────────────┐
│ Nova Partitura - Passo 1        │
│ Selecione instrumentos          │
│                                 │
│ Biblioteca de Instrumentos      │
│ [Buscar...]                     │
│ ┌─────────────────────────────┐ │
│ │ • Flute       (C4-C7)   [+] │ │
│ │ • Oboe        (Bb3-A6)  [+] │ │
│ │ • Trumpet     (F#3-D6)  [+] │ │
│ │ • Violin      (G3-A7)   [+] │ │
│ │ • Cello       (C2-E5)   [+] │ │
│ │ • Snare Drum  (Perc)    [+] │ │
│ └─────────────────────────────┘ │
│                                 │
│ Selecionados:                   │
│ • Violin                        │
│ • Cello                         │
│                                 │
│ Modo Performance                │
│ ☑ Modo Compatibilidade          │
│ ☑ Preview ao vivo               │
│                                 │
│          [Anterior] [Próximo →] │
└─────────────────────────────────┘
```

**Recursos:**
- Lista virtualizada (lazy-loading)
- Busca em tempo real
- Drag & drop para reordenar (opcional)
- Toggle para modo compatibilidade

### Passo 2: Configurações da Partitura

**Layout:**
```
┌─────────────────────────────────┐
│ Nova Partitura - Passo 2        │
│ Configurar partitura            │
│                                 │
│ Armadura:                       │
│ [C Major ▼]                     │
│                                 │
│ Compasso:                       │
│ [4/4 ▼]                         │
│                                 │
│ Andamento (BPM):                │
│ [●───────-] 120                 │
│                                 │
│ Título:                         │
│ [Symphony in D Minor──────────] │
│                                 │
│ Compositor:                     │
│ [John Composer────────────────] │
│                                 │
│          [← Anterior] [Criar →] │
└─────────────────────────────────┘
```

## ⚡ Otimizações de Performance

### Modo Compatibilidade

Quando ativado, reduz significativamente o consumo de GPU:

```cpp
if (g_settings.compatibilityMode) {
    // Desativar:
    g_settings.disableAnimations = true;
    g_settings.disableShadows = true;
    g_settings.disableSmoothZoom = true;
    
    // Usar renderização vetorial simplificada
    g_settings.useSimplifiedRendering = true;
    
    // Preview estático em vez de real-time
    g_settings.livePreviewDisabled = true;
}
```

### Memória

- **Idle**: <200MB
- **Com partitura aberta**: ~200-400MB
- **Modo compatibilidade**: Reduz ~50%

**Técnicas:**
- Object pooling para widgets efêmeros
- Lazy-loading de imagens/ícones
- Virtualização de listas
- Release de recursos após uso

### CPU

- Interface 60fps em GPU integrada
- Renderização não-bloqueante
- Threads assíncronas para operações pesadas
- Cache de renderização de score

## 🎨 Customização de Tema

### Criar novo tema

```cpp
// Em Theme.hpp, adicione:
struct CustomThemePalette {
    static constexpr Color Primary{/* R */ 100, /* G */ 50, /* B */ 200, 255};
    // ... outras cores
};

// Então use:
struct Theme {
    void setCustomMode(bool custom) {
        if (custom) {
            accentColor = CustomThemePalette::Primary;
            // ...
        }
    }
};
```

### Modificar stylesheet em runtime

```cpp
void applyCustomStylesheet(const QString& qssCode) {
    qApp->setStyleSheet(qssCode);
}

// Exemplo: aumentar tamanho de fonte
QString customStyle = R"(
    QPushButton { font-size: 16px; }
    QLabel { font-size: 13px; }
)";
applyCustomStylesheet(customStyle);
```

## 🔧 Troubleshooting

### Interface lenta em PC fraco
1. Ativar "Modo Compatibilidade"
2. Desativar preview ao vivo
3. Verificar: `g_settings.disableAnimations = true`

### Tema não aplica
```cpp
// Certifique-se de chamar antes de criar widgets:
g_theme.setMode(ThemeMode::Dark);
qApp->setStyleSheet(loadStylesheet());
```

### Widgets renderizando incorretamente
- Verificar se `painter.setRenderHint(QPainter::Antialiasing)` está ativo
- Chamar `update()` ou `repaint()` após mudanças de estado

## 📦 Dependências

- Qt 6.x (Core, Gui, Widgets)
- C++20
- CMake 3.20+

## 🚀 Build e Deploy

### Build com interface Qt

```bash
cmake -S . -B build -DNOTASCORE_ENABLE_QT=ON
cmake --build build --config Release
./build/NotaScore
```

### AppImage (Linux)

```bash
./packaging/linux/build_appimage.sh
# Resulta em: NotaScore-x86_64.AppImage
```

## 📚 Referências

- **Windows 11 Design System**: Microsoft Design
- **Qt Documentation**: https://doc.qt.io/qt-6/
- **Modern C++ Practices**: C++20 Standards
- **Performance**: Intel HD integrated GPU best practices

---

**Última atualização**: 2026-02-19  
**Versão do NotaScore**: 0.1.0
