#ifdef NOTASCORE_ENABLE_QT

#include "notascore/ui/QtMainWindow.hpp"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QListView>
#include <QListWidget>
#include <QLineEdit>
#include <QLabel>
#include <QCheckBox>
#include <QDockWidget>
#include <QFile>
#include <QTimer>

namespace notascore::ui {

QtMainWindow::QtMainWindow(notascore::ui::MainWindow& view, QWidget* parent)
    : QMainWindow(parent), m_view(view), m_instrumentModel(nullptr), m_preview(nullptr), m_selectedList(nullptr), m_recentList(nullptr) {
    setWindowTitle("NotaScore");
    resize(m_view.width(), m_view.height());

    auto* central = new QWidget;
    auto* mainLayout = new QHBoxLayout(central);

    // Left area: main card + recent
    auto* left = new QWidget;
    auto* leftV = new QVBoxLayout(left);

    // Top menu is handled by QMainWindow menuBar
    auto* menu = menuBar();
    menu->addMenu("Arquivo");
    menu->addMenu("Editar");
    menu->addMenu("Preferencias");
    menu->addMenu("Ajuda");

    // Main card
    auto* card = new QWidget;
    card->setObjectName("mainCard");
    auto* cardL = new QVBoxLayout(card);
    auto* newBtn = new QPushButton("Nova Partitura");
    newBtn->setObjectName("primary");
    newBtn->setFixedHeight(48);
    cardL->addStretch();
    cardL->addWidget(newBtn, 0, Qt::AlignCenter);
    cardL->addStretch();

    leftV->addWidget(card, 0, Qt::AlignCenter);

    // Recent projects
    auto* recentLabel = new QLabel("Projetos Recentes");
    m_recentList = new QListWidget;
    m_recentList->setMaximumHeight(220);
    leftV->addWidget(recentLabel);
    leftV->addWidget(m_recentList);

    mainLayout->addWidget(left, 3);

    // Assistant panel on the right
    auto* assistant = new QWidget;
    assistant->setFixedWidth(420);
    auto* asL = new QVBoxLayout(assistant);
    auto* title = new QLabel("Nova Partitura");
    asL->addWidget(title);

    // Instrument selection
    auto* libLabel = new QLabel("Biblioteca");
    auto* search = new QLineEdit;
    search->setPlaceholderText("Buscar instrumentos");
    auto* listView = new QListView;
    m_instrumentModel = new InstrumentModel(const_cast<std::vector<InstrumentDef>&>(m_view.instrumentLibrary()), this);
    listView->setModel(m_instrumentModel);
    listView->setSelectionMode(QAbstractItemView::SingleSelection);
    asL->addWidget(libLabel);
    asL->addWidget(search);
    asL->addWidget(listView);

    // Selected instruments
    auto* selLabel = new QLabel("Selecionados");
    m_selectedList = new QListWidget;
    m_selectedList->setSelectionMode(QAbstractItemView::SingleSelection);
    asL->addWidget(selLabel);
    asL->addWidget(m_selectedList);

    // Toggles
    auto* compat = new QCheckBox("Modo Compatibilidade");
    auto* live = new QCheckBox("Preview ao vivo");
    compat->setChecked(m_view.compatibilityMode());
    live->setChecked(m_view.livePreviewEnabled());
    asL->addWidget(compat);
    asL->addWidget(live);

    // Controls
    auto* back = new QPushButton("Voltar");
    auto* next = new QPushButton("Proximo");
    back->setFixedHeight(34);
    next->setFixedHeight(34);
    asL->addStretch();
    asL->addWidget(back);
    asL->addWidget(next);

    mainLayout->addWidget(assistant, 0);

    setCentralWidget(central);

    // Preview widget bottom-right (dock-like)
    m_preview = new PreviewWidget(this);
    m_preview->setFixedSize(200, 140);
    m_preview->move(width() - m_preview->width() - 16, height() - m_preview->height() - 36);
    m_preview->setParent(this);
    m_preview->show();

    // Load stylesheet if available
    QFile f(":/resources/styles.qss");
    if (!f.exists()) {
        // try relative path
        QFile f2(QString::fromLatin1("/workspaces/NotaScore/resources/styles.qss"));
        if (f2.open(QFile::ReadOnly | QFile::Text)) {
            qApp->setStyleSheet(QString::fromUtf8(f2.readAll()));
        }
    } else if (f.open(QFile::ReadOnly | QFile::Text)) {
        qApp->setStyleSheet(QString::fromUtf8(f.readAll()));
    }

    // Connections
    connect(newBtn, &QPushButton::clicked, this, [&]() {
        const auto r = m_view.newScoreCardRect();
        m_view.onClick(r.x + 8, r.y + 8);
        refresh();
    });

    connect(listView, &QListView::doubleClicked, this, [this](const QModelIndex& idx) {
        if (!idx.isValid()) return;
        const auto i = static_cast<std::size_t>(idx.row());
        const auto r = m_view.instrumentAddRect(i);
        m_view.onClick(r.x + 6, r.y + 6);
        refresh();
    });

    connect(m_selectedList, &QListWidget::itemDoubleClicked, this, [this](QListWidgetItem* it) {
        const auto name = it->text().toStdString();
        // find index in selected instruments
        for (std::size_t i = 0; i < m_view.selectedInstruments().size(); ++i) {
            if (m_view.selectedInstruments()[i].name == name) {
                const auto r = m_view.selectedInstrumentRemoveRect(i);
                m_view.onClick(r.x + 4, r.y + 4);
                break;
            }
        }
        refresh();
    });

    connect(compat, &QCheckBox::toggled, this, [this](bool) {
        const auto r = m_view.compatibilityToggleRect();
        m_view.onClick(r.x + 2, r.y + 2);
        refresh();
    });

    connect(live, &QCheckBox::toggled, this, [this](bool) {
        const auto r = m_view.livePreviewToggleRect();
        m_view.onClick(r.x + 2, r.y + 2);
        refresh();
    });

    connect(back, &QPushButton::clicked, this, [this]() {
        const auto r = m_view.assistantBackRect();
        m_view.onClick(r.x + 4, r.y + 4);
        refresh();
    });

    connect(next, &QPushButton::clicked, this, [this]() {
        const auto r = m_view.assistantNextRect();
        m_view.onClick(r.x + 4, r.y + 4);
        refresh();
    });

    // Periodic refresh to reflect view model changes
    QTimer* t = new QTimer(this);
    connect(t, &QTimer::timeout, this, &QtMainWindow::refresh);
    t->start(250);

    refresh();
}

void QtMainWindow::refresh() {
    // Recent projects
    m_recentList->clear();
    for (const auto& p : m_view.recentProjects()) m_recentList->addItem(QString::fromStdString(p));

    // Selected instruments
    m_selectedList->clear();
    for (const auto& s : m_view.selectedInstruments()) m_selectedList->addItem(QString::fromStdString(s.name));

    // Preview visibility
    m_preview->setVisible(m_view.livePreviewEnabled());
}

} // namespace notascore::ui

#endif // NOTASCORE_ENABLE_QT
