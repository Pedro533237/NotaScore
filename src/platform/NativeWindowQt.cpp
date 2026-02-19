#ifdef NOTASCORE_ENABLE_QT

#include "notascore/platform/NativeWindow.hpp"

#include <QApplication>
#include <QMainWindow>
#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QListWidget>
#include <QMenuBar>
#include <QTimer>
#include <QLabel>

namespace notascore::platform {

NativeWindow::NativeWindow(notascore::ui::MainWindow& view)
    : m_view(view) {}

int NativeWindow::run() {
    int argc = 0;
    char** argv = nullptr;
    QApplication app(argc, argv);

    QMainWindow window;
    window.setWindowTitle("NotaScore");
    window.resize(m_view.width(), m_view.height());

    QWidget* central = new QWidget;
    auto* layout = new QVBoxLayout(central);

    // Menu
    auto* menu = new QMenuBar(&window);
    menu->addMenu("Arquivo");
    menu->addMenu("Editar");
    menu->addMenu("Preferencias");
    menu->addMenu("Ajuda");
    window.setMenuBar(menu);

    // Card principal
    auto* card = new QWidget;
    card->setObjectName("mainCard");
    auto* cardLayout = new QVBoxLayout(card);
    auto* newBtn = new QPushButton("Nova Partitura");
    newBtn->setObjectName("primary");
    cardLayout->addWidget(newBtn, 0, Qt::AlignCenter);
    layout->addWidget(card, 0, Qt::AlignCenter);

    // Recent projects
    auto* recent = new QListWidget;
    for (const auto& p : m_view.recentProjects()) {
        recent->addItem(QString::fromStdString(p));
    }
    recent->setMaximumHeight(200);
    layout->addWidget(recent);

    central->setLayout(layout);
    window.setCentralWidget(central);

    // Style (light theme default)
    app.setStyleSheet(R"(
        QWidget { background: #F3F3F3; color: #1A1A1A; }
        #mainCard { background: #FFFFFF; border-radius: 12px; border: 1px solid #E0E0E0; padding: 12px; }
        QPushButton#primary { background: #2563EB; color: white; border-radius: 10px; padding: 12px 18px; }
        QPushButton#primary:hover { background: #1e50c9; }
    )");

    // Button action: simulate click into the view model
    QObject::connect(newBtn, &QPushButton::clicked, [&](){
        const auto r = m_view.newScoreCardRect();
        m_view.onClick(r.x + 8, r.y + 8);
    });

    // Poll status text to update window title (lightweight)
    QTimer timer;
    QObject::connect(&timer, &QTimer::timeout, [&](){
        window.setWindowTitle(QString::fromStdString(std::string("NotaScore - ") + m_view.statusText()));
    });
    timer.start(250);

    window.show();
    return app.exec();
}

} // namespace notascore::platform

#endif // NOTASCORE_ENABLE_QT
