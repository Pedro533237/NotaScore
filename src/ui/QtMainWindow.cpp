#ifdef NOTASCORE_ENABLE_QT

#include "notascore/ui/QtMainWindow.hpp"
#include "notascore/ui/ModernWidgets.hpp"
#include "notascore/ui/Theme.hpp"

#include <QApplication>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QScrollArea>
#include <QFile>
#include <QTimer>
#include <QIcon>
#include <QPixmap>
#include <QPainter>

namespace notascore::ui {

// Helper to create a musical note icon
static QIcon createNoteIcon() {
    QPixmap pixmap(32, 32);
    pixmap.fill(Qt::transparent);
    
    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QPen(g_theme.accentColor, 2));
    
    // Simple note head and stem
    painter.drawEllipse(4, 18, 8, 8);
    painter.drawLine(12, 10, 12, 18);
    painter.drawLine(12, 10, 20, 8);
    
    return QIcon(pixmap);
}

QtMainWindow::QtMainWindow(notascore::ui::MainWindow& view, QWidget* parent)
    : QMainWindow(parent), m_view(view), m_instrumentModel(nullptr), 
      m_preview(nullptr), m_selectedList(nullptr), m_recentList(nullptr) {
    
    setWindowTitle("NotaScore - Professional Musical Notation Editor");
    setWindowIcon(createNoteIcon());
    resize(1200, 800);

    // Setup theme from UI settings
    if (view.compatibilityMode()) {
        g_theme.setMode(ThemeMode::Light);
    }

    // Create main widget
    auto* central = new QWidget;
    auto* mainLayout = new QHBoxLayout(central);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // ========== TOP MENU BAR ==========
    auto* menu = menuBar();
    menu->setAttribute(Qt::WA_NoMousePropagation);
    
    auto* fileMenu = menu->addMenu(tr("Arquivo"));
    fileMenu->addAction(tr("Nova Partitura"));
    fileMenu->addAction(tr("Abrir Projeto..."));
    fileMenu->addAction(tr("Importar MIDI..."));
    fileMenu->addAction(tr("Importar MusicXML..."));
    fileMenu->addSeparator();
    fileMenu->addAction(tr("Sair"));

    auto* editMenu = menu->addMenu(tr("Editar"));
    editMenu->addAction(tr("Desfazer"));
    editMenu->addAction(tr("Refazer"));
    editMenu->addSeparator();
    editMenu->addAction(tr("Preferências"));

    auto* helpMenu = menu->addMenu(tr("Ajuda"));
    helpMenu->addAction(tr("Documentação"));
    helpMenu->addAction(tr("Sobre o NotaScore"));

    // ========== LEFT PANEL: HOME SCREEN ==========
    auto* leftPanel = new QWidget;
    leftPanel->setMinimumWidth(740);
    auto* leftLayout = new QVBoxLayout(leftPanel);
    leftLayout->setContentsMargins(24, 24, 24, 24);
    leftLayout->setSpacing(24);

    // Logo and title
    auto* headerWidget = new QWidget;
    auto* headerLayout = new QVBoxLayout(headerWidget);
    headerLayout->setContentsMargins(0, 0, 0, 0);
    
    auto* logoLabel = new QLabel("🎼");
    logoLabel->setStyleSheet("font-size: 48px;");
    auto* titleLabel = new QLabel("NotaScore");
    titleLabel->setStyleSheet("font-size: 32px; font-weight: bold; color: #1A1A1A;");
    auto* subtitleLabel = new QLabel(tr("Editor profissional de notação musical"));
    subtitleLabel->setStyleSheet("font-size: 14px; color: #6E6E6E;");
    
    headerLayout->addWidget(logoLabel, 0, Qt::AlignCenter);
    headerLayout->addWidget(titleLabel, 0, Qt::AlignCenter);
    headerLayout->addWidget(subtitleLabel, 0, Qt::AlignCenter);
    
    leftLayout->addLayout(headerLayout);
    leftLayout->addSpacing(16);

    // Main action card
    auto* mainCard = new ModernCard;
    mainCard->setTitle(tr("Nova Partitura"));
    mainCard->setSubtitle(tr("Criar uma nova partitura em branco"));
    mainCard->setIcon(createNoteIcon());
    mainCard->setMinimumHeight(180);
    leftLayout->addWidget(mainCard, 0, Qt::AlignCenter);

    // Quick action buttons
    auto* quickActionsWidget = new QWidget;
    auto* quickLayout = new QHBoxLayout(quickActionsWidget);
    quickLayout->setSpacing(12);
    
    auto* openBtn = new ModernButton(tr("Abrir Projeto"), ModernButton::Style::Secondary);
    auto* importMidiBtn = new ModernButton(tr("Importar MIDI"), ModernButton::Style::Secondary);
    auto* importXmlBtn = new ModernButton(tr("Importar MusicXML"), ModernButton::Style::Secondary);
    
    quickLayout->addWidget(openBtn);
    quickLayout->addWidget(importMidiBtn);
    quickLayout->addWidget(importXmlBtn);
    quickLayout->addStretch();
    
    leftLayout->addWidget(quickActionsWidget);

    // Recent projects section
    auto* recentTitleLabel = new QLabel(tr("Projetos Recentes"));
    recentTitleLabel->setStyleSheet("font-size: 14px; font-weight: 600; color: #1A1A1A;");
    leftLayout->addWidget(recentTitleLabel);

    m_recentList = new QListWidget;
    m_recentList->setStyleSheet(
        "QListWidget { background-color: #FFFFFF; border: 1px solid #E0E0E0; border-radius: 8px; }"
        "QListWidget::item { padding: 8px; border-bottom: 1px solid #E0E0E0; }"
        "QListWidget::item:hover { background-color: #E5F0FF; }"
        "QListWidget::item:selected { background-color: #E5F0FF; color: #2563EB; }"
    );
    m_recentList->setMaximumHeight(200);
    leftLayout->addWidget(m_recentList);

    leftLayout->addStretch();

    mainLayout->addWidget(leftPanel, 3);

    // ========== RIGHT PANEL: WIZARD ASSISTANT ==========
    auto* assistantPanel = new QWidget;
    assistantPanel->setFixedWidth(420);
    assistantPanel->setStyleSheet(
        "QWidget { background-color: #FFFFFF; border-left: 1px solid #E0E0E0; }"
    );
    
    auto* assistantLayout = new QVBoxLayout(assistantPanel);
    assistantLayout->setContentsMargins(24, 24, 24, 24);
    assistantLayout->setSpacing(16);

    // Title
    auto* wizardTitle = new QLabel(tr("Nova Partitura - Passo 1"));
    wizardTitle->setStyleSheet("font-size: 16px; font-weight: 600; color: #1A1A1A;");
    assistantLayout->addWidget(wizardTitle);

    // Step indicator
    auto* stepLabel = new QLabel(tr("Selecione instrumentos"));
    stepLabel->setStyleSheet("font-size: 12px; color: #6E6E6E;");
    assistantLayout->addWidget(stepLabel);

    assistantLayout->addSpacing(8);

    // Instrument library
    auto* libTitleLabel = new QLabel(tr("Biblioteca de Instrumentos"));
    libTitleLabel->setStyleSheet("font-size: 12px; font-weight: 500; color: #6E6E6E;");
    assistantLayout->addWidget(libTitleLabel);

    auto* searchInput = new ModernLineEdit;
    searchInput->setPlaceholder(tr("Buscar instrumentos..."));
    assistantLayout->addWidget(searchInput);

    auto* instrumentListScroll = new QScrollArea;
    instrumentListScroll->setWidgetResizable(true);
    instrumentListScroll->setStyleSheet("QScrollArea { border: 1px solid #E0E0E0; border-radius: 6px; }");
    
    auto* instrumentListContainer = new QWidget;
    auto* instrumentListLayout = new QVBoxLayout(instrumentListContainer);
    
    for (std::size_t i = 0; i < std::min<std::size_t>(6, m_view.instrumentLibrary().size()); ++i) {
        const auto& inst = m_view.instrumentLibrary()[i];
        
        auto* instWidget = new QWidget;
        auto* instLayout = new QHBoxLayout(instWidget);
        instLayout->setContentsMargins(8, 6, 8, 6);
        
        auto* nameLabel = new QLabel(QString::fromStdString(inst.name));
        nameLabel->setStyleSheet("font-size: 12px; color: #1A1A1A;");
        
        auto* rangeLabel = new QLabel(QString::fromStdString(inst.range));
        rangeLabel->setStyleSheet("font-size: 10px; color: #6E6E6E;");
        
        auto* addBtn = new ModernButton("+", ModernButton::Style::Subtle);
        addBtn->setFixedWidth(32);
        
        instLayout->addWidget(nameLabel);
        instLayout->addWidget(rangeLabel);
        instLayout->addStretch();
        instLayout->addWidget(addBtn);
        
        instrumentListLayout->addWidget(instWidget);
    }
    
    instrumentListLayout->addStretch();
    instrumentListContainer->setLayout(instrumentListLayout);
    instrumentListScroll->setWidget(instrumentListContainer);
    assistantLayout->addWidget(instrumentListScroll);

    // Selected instruments
    auto* selectedTitleLabel = new QLabel(tr("Selecionados"));
    selectedTitleLabel->setStyleSheet("font-size: 12px; font-weight: 500; color: #6E6E6E;");
    assistantLayout->addWidget(selectedTitleLabel);

    m_selectedList = new QListWidget;
    m_selectedList->setStyleSheet(
        "QListWidget { background-color: #F3F3F3; border: 1px solid #E0E0E0; border-radius: 6px; }"
        "QListWidget::item { padding: 6px; }"
        "QListWidget::item:hover { background-color: #E5F0FF; }"
    );
    m_selectedList->setMaximumHeight(120);
    assistantLayout->addWidget(m_selectedList);

    // Performance settings
    assistantLayout->addSpacing(12);
    auto* performanceTitle = new QLabel(tr("Modo Performance"));
    performanceTitle->setStyleSheet("font-size: 12px; font-weight: 500; color: #6E6E6E;");
    assistantLayout->addWidget(performanceTitle);

    auto* compatToggle = new PerformanceToggle(tr("Modo Compatibilidade"));
    compatToggle->setDescription(tr("Disativa sombras e animações"));
    assistantLayout->addWidget(compatToggle);

    auto* previewToggle = new PerformanceToggle(tr("Preview ao vivo"));
    previewToggle->setDescription(tr("Renderizar preview em tempo real"));
    previewToggle->setChecked(true);
    assistantLayout->addWidget(previewToggle);

    assistantLayout->addStretch();

    // Navigation buttons
    auto* navWidget = new QWidget;
    auto* navLayout = new QHBoxLayout(navWidget);
    navLayout->setContentsMargins(0, 0, 0, 0);
    navLayout->setSpacing(8);
    
    auto* backBtn = new ModernButton(tr("← Anterior"), ModernButton::Style::Secondary);
    auto* nextBtn = new ModernButton(tr("Próximo →"), ModernButton::Style::Primary);
    backBtn->setFixedHeight(40);
    nextBtn->setFixedHeight(40);
    
    navLayout->addWidget(backBtn);
    navLayout->addWidget(nextBtn);
    
    assistantLayout->addWidget(navWidget);

    mainLayout->addWidget(assistantPanel);

    setCentralWidget(central);

    // Load modern stylesheet
    loadModernStylesheet();

    // Preview widget
    m_preview = new PreviewWidget(this);
    m_preview->setFixedSize(200, 140);
    m_preview->move(width() - m_preview->width() - 16, height() - m_preview->height() - 36);
    m_preview->setParent(this);
    m_preview->show();

    // Connections
    connect(mainCard, &ModernCard::clicked, this, [&]() {
        const auto r = m_view.newScoreCardRect();
        m_view.onClick(r.x + 8, r.y + 8);
        refresh();
    });

    connect(openBtn, &ModernButton::clicked, this, [this]() {
        const auto r = m_view.openProjectRect();
        m_view.onClick(r.x + 4, r.y + 4);
        refresh();
    });

    connect(importMidiBtn, &ModernButton::clicked, this, [this]() {
        const auto r = m_view.importMidiRect();
        m_view.onClick(r.x + 4, r.y + 4);
        refresh();
    });

    connect(importXmlBtn, &ModernButton::clicked, this, [this]() {
        const auto r = m_view.importMusicXmlRect();
        m_view.onClick(r.x + 4, r.y + 4);
        refresh();
    });

    connect(compatToggle, &PerformanceToggle::toggled, this, [this](bool) {
        const auto r = m_view.compatibilityToggleRect();
        m_view.onClick(r.x + 2, r.y + 2);
        refresh();
    });

    connect(previewToggle, &PerformanceToggle::toggled, this, [this](bool) {
        const auto r = m_view.livePreviewToggleRect();
        m_view.onClick(r.x + 2, r.y + 2);
        refresh();
    });

    connect(backBtn, &ModernButton::clicked, this, [this]() {
        const auto r = m_view.assistantBackRect();
        m_view.onClick(r.x + 4, r.y + 4);
        refresh();
    });

    connect(nextBtn, &ModernButton::clicked, this, [this]() {
        const auto r = m_view.assistantNextRect();
        m_view.onClick(r.x + 4, r.y + 4);
        refresh();
    });

    // Periodic refresh
    QTimer* refreshTimer = new QTimer(this);
    connect(refreshTimer, &QTimer::timeout, this, &QtMainWindow::refresh);
    refreshTimer->start(250);

    refresh();
}

void QtMainWindow::loadModernStylesheet() {
    QFile f(":/resources/styles-modern.qss");
    if (f.open(QFile::ReadOnly | QFile::Text)) {
        qApp->setStyleSheet(QString::fromUtf8(f.readAll()));
        f.close();
    }
}

void QtMainWindow::refresh() {
    // Update recent projects list
    m_recentList->clear();
    for (const auto& project : m_view.recentProjects()) {
        m_recentList->addItem(QString::fromStdString(project));
    }

    // Update selected instruments list
    m_selectedList->clear();
    for (const auto& inst : m_view.selectedInstruments()) {
        m_selectedList->addItem(QString::fromStdString(inst.name));
    }

    // Update preview visibility
    m_preview->setVisible(m_view.livePreviewEnabled());

    // Force paint update for theme changes
    update();
}

} // namespace notascore::ui

#endif // NOTASCORE_ENABLE_QT
