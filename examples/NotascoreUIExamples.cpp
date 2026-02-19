/**
 * @file NotaScore Modern Interface - Usage Examples
 * 
 * Exemplos de código mostrando como usar a interface moderna do NotaScore.
 */

#ifdef NOTASCORE_ENABLE_QT

#include "notascore/ui/Theme.hpp"
#include "notascore/ui/ModernWidgets.hpp"
#include "notascore/ui/QtMainWindow.hpp"

#include <QApplication>
#include <QMainWindow>
#include <QVBoxLayout>

// ============================================================================
// EXEMPLO 1: Criar uma janela com tema moderno
// ============================================================================

void example_basic_window() {
    // Criar aplicação Qt
    int argc = 0;
    char* argv[] = {};
    QApplication app(argc, argv);
    
    // Definir tema (automático baseado no sistema)
    notascore::ui::g_theme.setMode(notascore::ui::ThemeMode::Light);
    
    // Criar janela principal
    QMainWindow window;
    window.setWindowTitle("Minha Aplicação NotaScore");
    window.setGeometry(100, 100, 1200, 800);
    
    // Criar layout central
    auto* central = new QWidget(&window);
    auto* layout = new QVBoxLayout(central);
    
    window.setCentralWidget(central);
    window.show();
    
    app.exec();
}

// ============================================================================
// EXEMPLO 2: Usar componentes customizados
// ============================================================================

void example_custom_widgets() {
    int argc = 0;
    char* argv[] = {};
    QApplication app(argc, argv);
    
    QMainWindow window;
    auto* central = new QWidget(&window);
    auto* layout = new QVBoxLayout(central);
    
    // Criar card (como no exemplo NotaScore)
    auto* card = new notascore::ui::ModernCard(&window);
    card->setTitle("Nova Partitura");
    card->setSubtitle("Criar uma nova partitura em branco");
    layout->addWidget(card);
    
    // Criar botão primário
    auto* btnCreate = new notascore::ui::ModernButton(
        "Criar",
        notascore::ui::ModernButton::Style::Primary
    );
    layout->addWidget(btnCreate);
    
    // Criar botão secundário
    auto* btnCancel = new notascore::ui::ModernButton(
        "Cancelar",
        notascore::ui::ModernButton::Style::Secondary
    );
    layout->addWidget(btnCancel);
    
    // Conectar sinais
    QObject::connect(btnCreate, &notascore::ui::ModernButton::clicked, 
                    [&]() { qDebug() << "Create clicked"; });
    QObject::connect(btnCancel, &notascore::ui::ModernButton::clicked,
                    [&]() { window.close(); });
    
    window.setCentralWidget(central);
    window.show();
    
    app.exec();
}

// ============================================================================
// EXEMPLO 3: Usar formulário com ModernLineEdit
// ============================================================================

void example_form_with_inputs() {
    int argc = 0;
    char* argv[] = {};
    QApplication app(argc, argv);
    
    QMainWindow window;
    auto* central = new QWidget(&window);
    auto* layout = new QVBoxLayout(central);
    
    // Título da partitura
    auto* titleInput = new notascore::ui::ModernLineEdit(&window);
    titleInput->setLabel("Título da Partitura");
    titleInput->setPlaceholder("Ex: Symphony in D Minor");
    layout->addWidget(titleInput);
    
    // Compositor
    auto* composerInput = new notascore::ui::ModernLineEdit(&window);
    composerInput->setLabel("Compositor");
    composerInput->setPlaceholder("Ex: Ludwig van Beethoven");
    layout->addWidget(composerInput);
    
    // Slider para BPM
    auto* tempoSlider = new notascore::ui::ModernSlider(&window);
    tempoSlider->setLabel("Andamento (BPM)");
    tempoSlider->setRange(40, 240);
    tempoSlider->setValue(120);
    layout->addWidget(tempoSlider);
    
    // Conectar e monitorar mudanças
    QObject::connect(titleInput, &notascore::ui::ModernLineEdit::textChanged,
                    [](const QString& text) {
                        qDebug() << "Título:" << text;
                    });
    
    QObject::connect(tempoSlider, qOverload<int>(&notascore::ui::ModernSlider::valueChanged),
                    [](int value) {
                        qDebug() << "BPM:" << value;
                    });
    
    layout->addStretch();
    
    window.setCentralWidget(central);
    window.resize(500, 400);
    window.show();
    
    app.exec();
}

// ============================================================================
// EXEMPLO 4: Modo compatibilidade automático
// ============================================================================

void example_performance_mode() {
    int argc = 0;
    char* argv[] = {};
    QApplication app(argc, argv);
    
    // Detectar hardware fraco
    bool isWeakHardware = true;  // Simulação
    
    if (isWeakHardware) {
        // Ativar modo compatibilidade
        notascore::ui::g_theme.setMode(notascore::ui::ThemeMode::Light);
        
        qDebug() << "Modo Compatibilidade ativado:";
        qDebug() << "  - Animações: desativadas";
        qDebug() << "  - Sombras: desativadas";
        qDebug() << "  - Antialiasing: desativado";
        qDebug() << "  - Preview ao vivo: desativado";
    }
    
    QMainWindow window;
    window.setWindowTitle("NotaScore - Modo Compatível");
    window.setGeometry(100, 100, 800, 600);
    window.show();
    
    app.exec();
}

// ============================================================================
// EXEMPLO 5: Alternar tema dinamicamente
// ============================================================================

void example_theme_switching() {
    int argc = 0;
    char* argv[] = {};
    QApplication app(argc, argv);
    
    QMainWindow window;
    auto* central = new QWidget(&window);
    auto* layout = new QVBoxLayout(central);
    
    auto* labelTheme = new QLabel("Tema Atual: Claro");
    layout->addWidget(labelTheme);
    
    // Botão para togglear tema
    auto* btnToggle = new notascore::ui::ModernButton(
        "Alternar Tema",
        notascore::ui::ModernButton::Style::Primary
    );
    layout->addWidget(btnToggle);
    
    QObject::connect(btnToggle, &notascore::ui::ModernButton::clicked, [&]() {
        bool isDark = notascore::ui::g_theme.isDark();
        
        if (isDark) {
            notascore::ui::g_theme.setMode(notascore::ui::ThemeMode::Light);
            labelTheme->setText("Tema Atual: Claro");
        } else {
            notascore::ui::g_theme.setMode(notascore::ui::ThemeMode::Dark);
            labelTheme->setText("Tema Atual: Escuro");
        }
        
        // Forçar repaint
        for (auto* widget : app.allWidgets()) {
            widget->update();
        }
    });
    
    layout->addStretch();
    
    window.setCentralWidget(central);
    window.resize(400, 300);
    window.show();
    
    app.exec();
}

// ============================================================================
// EXEMPLO 6: Performance Toggles
// ============================================================================

void example_performance_toggles() {
    int argc = 0;
    char* argv[] = {};
    QApplication app(argc, argv);
    
    QMainWindow window;
    auto* central = new QWidget(&window);
    auto* layout = new QVBoxLayout(central);
    
    // Toggle para compatibilidade
    auto* compatToggle = new notascore::ui::PerformanceToggle(
        "Modo Compatibilidade"
    );
    compatToggle->setDescription("Desativa sombras, animações e preview ao vivo");
    layout->addWidget(compatToggle);
    
    // Toggle para preview
    auto* previewToggle = new notascore::ui::PerformanceToggle(
        "Preview ao Vivo"
    );
    previewToggle->setDescription("Renderizar partitura em tempo real");
    previewToggle->setChecked(true);
    layout->addWidget(previewToggle);
    
    // Conectar
    QObject::connect(compatToggle, &notascore::ui::PerformanceToggle::toggled,
                    [](bool checked) {
                        qDebug() << "Compatibilidade:" << (checked ? "ON" : "OFF");
                    });
    
    QObject::connect(previewToggle, &notascore::ui::PerformanceToggle::toggled,
                    [](bool checked) {
                        qDebug() << "Preview:" << (checked ? "ON" : "OFF");
                    });
    
    layout->addStretch();
    
    window.setCentralWidget(central);
    window.resize(500, 400);
    window.show();
    
    app.exec();
}

// ============================================================================
// EXEMPLO 7: Acessar cores do tema
// ============================================================================

void example_theme_colors() {
    // Após g_theme estar configurado:
    
    notascore::ui::Color bgMain = notascore::ui::g_theme.backgroundMain;
    notascore::ui::Color accent = notascore::ui::g_theme.accentColor;
    notascore::ui::Color textPrimary = notascore::ui::g_theme.textPrimary;
    
    // Usar cores em renderização customizada
    QPainter painter;
    
    // Light theme
    if (notascore::ui::g_theme.isLight()) {
        painter.fillRect(rect, QColor(bgMain.r, bgMain.g, bgMain.b, bgMain.a));
        painter.setPen(QColor(textPrimary.r, textPrimary.g, textPrimary.b));
    } else {
        painter.fillRect(rect, QColor(bgMain.r, bgMain.g, bgMain.b, bgMain.a));
        painter.setPen(QColor(textPrimary.r, textPrimary.g, textPrimary.b));
    }
}

// ============================================================================
// EXEMPLO 8: Integração completa NotaScore
// ============================================================================

void example_full_notascore_integration() {
    int argc = 0;
    char* argv[] = {};
    
    // Inicializar view model
    notascore::core::PerformanceSettings perfSettings;
    perfSettings.cpuModeOnly = false;
    perfSettings.disableAnimations = false;
    
    notascore::ui::MainWindow viewModel(1200, 800, perfSettings);
    
    // Criar aplicação Qt
    QApplication app(argc, argv);
    
    // Criar janela principal (usa widgets customizados automaticamente)
    notascore::ui::QtMainWindow mainWindow(viewModel);
    mainWindow.show();
    
    // Simular clique em "Nova Partitura"
    auto r = viewModel.newScoreCardRect();
    viewModel.onClick(r.x + 8, r.y + 8);
    
    mainWindow.refresh();
    
    app.exec();
}

// ============================================================================

/**
 * Para compilar e rodar um destes exemplos:
 * 
 * 1. Adicionar ao CMakeLists.txt:
 *    add_executable(example_ui examples/NotascoreExamples.cpp)
 *    target_link_libraries(example_ui PRIVATE notascore_engine)
 * 
 * 2. Compilar:
 *    cmake -S . -B build -DNOTASCORE_ENABLE_QT=ON
 *    cmake --build build
 * 
 * 3. Rodar:
 *    ./build/example_ui
 */

#endif // NOTASCORE_ENABLE_QT
