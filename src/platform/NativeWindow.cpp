#include "notascore/platform/NativeWindow.hpp"

#include <algorithm>
#include <iostream>
#include <string>

#if defined(NOTASCORE_ENABLE_QT)

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

#elif defined(_WIN32)

#include <windows.h>

namespace {

COLORREF rgb(unsigned char r, unsigned char g, unsigned char b) {
    return RGB(r, g, b);
}

void fillRect(HDC hdc, const notascore::ui::Rect& rect, COLORREF color) {
    RECT nativeRect {rect.x, rect.y, rect.x + rect.width, rect.y + rect.height};
    HBRUSH brush = CreateSolidBrush(color);
    FillRect(hdc, &nativeRect, brush);
    DeleteObject(brush);
}

void fillRoundedRect(HDC hdc, const notascore::ui::Rect& rect, COLORREF color, int radius) {
    HBRUSH brush = CreateSolidBrush(color);
    HPEN pen = CreatePen(PS_SOLID, 1, color);
    const auto oldBrush = SelectObject(hdc, brush);
    const auto oldPen = SelectObject(hdc, pen);
    RoundRect(hdc, rect.x, rect.y, rect.x + rect.width, rect.y + rect.height, radius, radius);
    SelectObject(hdc, oldPen);
    SelectObject(hdc, oldBrush);
    DeleteObject(pen);
    DeleteObject(brush);
}

void drawText(HDC hdc, int x, int y, COLORREF color, const std::string& text) {
    SetBkMode(hdc, TRANSPARENT);
    SetTextColor(hdc, color);
    TextOutA(hdc, x, y, text.c_str(), static_cast<int>(text.size()));
}

void drawMainHome(HDC hdc, const notascore::ui::MainWindow& view) {
    const auto width = view.width();
    const auto height = view.height();

    fillRect(hdc, {0, 0, width, height}, rgb(243, 243, 243));
    fillRect(hdc, {0, 0, width, 54}, rgb(255, 255, 255));
    fillRect(hdc, {0, 54, 210, height - 54}, rgb(255, 255, 255));

    drawText(hdc, 20, 18, rgb(26, 26, 26), "NotaScore");
    drawText(hdc, 160, 18, rgb(26, 26, 26), "Arquivo  Editar  Preferencias  Ajuda");

    fillRect(hdc, {0, 53, width, 1}, rgb(224, 224, 224));
    fillRect(hdc, {209, 54, 1, height - 54}, rgb(224, 224, 224));
    drawText(hdc, 24, 90, rgb(26, 26, 26), "Home");
    drawText(hdc, 24, 124, rgb(80, 80, 80), "Projetos");

    const auto card = view.newScoreCardRect();
    fillRoundedRect(hdc, card, rgb(37, 99, 235), 12);
    drawText(hdc, card.x + 56, card.y + 62, rgb(245, 245, 245), "Nova Partitura");

    fillRoundedRect(hdc, view.openProjectRect(), rgb(255, 255, 255), 10);
    fillRoundedRect(hdc, view.importMidiRect(), rgb(255, 255, 255), 10);
    fillRoundedRect(hdc, view.importMusicXmlRect(), rgb(255, 255, 255), 10);
    drawText(hdc, view.openProjectRect().x + 12, view.openProjectRect().y + 10, rgb(26, 26, 26), "Abrir Projeto");
    drawText(hdc, view.importMidiRect().x + 12, view.importMidiRect().y + 10, rgb(26, 26, 26), "Importar MIDI");
    drawText(hdc, view.importMusicXmlRect().x + 12, view.importMusicXmlRect().y + 10, rgb(26, 26, 26), "Importar MusicXML");

    drawText(hdc, 560, 106, rgb(26, 26, 26), "Projetos Recentes");
    int y = 136;
    for (const auto& item : view.recentProjects()) {
        fillRoundedRect(hdc, {560, y, 290, 32}, rgb(255, 255, 255), 8);
        drawText(hdc, 574, y + 9, rgb(26, 26, 26), item);
        y += 40;
        if (y > 300) {
            break;
        }
    }
}

void drawWizardPanel(HDC hdc, const notascore::ui::MainWindow& view) {
    const auto panel = view.assistantPanelRect();
    fillRect(hdc, panel, rgb(255, 255, 255));
    fillRect(hdc, {panel.x, 0, 1, panel.height}, rgb(224, 224, 224));

    drawText(hdc, panel.x + 24, 26, rgb(26, 26, 26), "Nova Partitura");
    const auto stepName = view.wizardStep() == notascore::ui::WizardStep::Instruments ? "1. Instrumentos" : "2. Configuracoes";
    drawText(hdc, panel.x + 24, 52, rgb(80, 80, 80), stepName);

    if (view.wizardStep() == notascore::ui::WizardStep::Instruments) {
        drawText(hdc, panel.x + 24, 84, rgb(26, 26, 26), "Biblioteca");
        drawText(hdc, panel.x + 216, 84, rgb(26, 26, 26), "Selecionados");

        const std::size_t visible = std::min<std::size_t>(6, view.instrumentLibrary().size());
        for (std::size_t i = 0; i < visible; ++i) {
            const int y = 110 + static_cast<int>(i) * 34;
            const auto& inst = view.instrumentLibrary()[i];
            drawText(hdc, panel.x + 24, y, rgb(26, 26, 26), inst.name);
            drawText(hdc, panel.x + 90, y, rgb(110, 110, 110), inst.range);
            fillRoundedRect(hdc, view.instrumentAddRect(i), rgb(229, 240, 255), 8);
            drawText(hdc, view.instrumentAddRect(i).x + 6, view.instrumentAddRect(i).y + 2, rgb(37, 99, 235), "+");
        }

        for (std::size_t i = 0; i < view.selectedInstruments().size(); ++i) {
            const int y = 110 + static_cast<int>(i) * 30;
            drawText(hdc, panel.x + 216, y, rgb(26, 26, 26), view.selectedInstruments()[i].name);
            fillRoundedRect(hdc, view.selectedInstrumentRemoveRect(i), rgb(255, 243, 243), 6);
            drawText(hdc, view.selectedInstrumentRemoveRect(i).x + 4, view.selectedInstrumentRemoveRect(i).y - 1, rgb(180, 40, 40), "x");
        }
    } else {
        drawText(hdc, panel.x + 24, 94, rgb(26, 26, 26), "Armadura");
        drawText(hdc, panel.x + 24, 154, rgb(26, 26, 26), "Compasso");
        drawText(hdc, panel.x + 24, 214, rgb(26, 26, 26), "Andamento");
        drawText(hdc, panel.x + 24, 274, rgb(26, 26, 26), "Titulo");
        drawText(hdc, panel.x + 24, 334, rgb(26, 26, 26), "Compositor");

        fillRoundedRect(hdc, view.keySignatureRect(), rgb(243, 243, 243), 8);
        fillRoundedRect(hdc, view.meterRect(), rgb(243, 243, 243), 8);
        fillRoundedRect(hdc, view.tempoRect(), rgb(243, 243, 243), 8);
        fillRoundedRect(hdc, view.titleRect(), rgb(243, 243, 243), 8);
        fillRoundedRect(hdc, view.composerRect(), rgb(243, 243, 243), 8);

        drawText(hdc, view.keySignatureRect().x + 12, view.keySignatureRect().y + 12, rgb(26, 26, 26), view.keySignature());
        drawText(hdc, view.meterRect().x + 12, view.meterRect().y + 12, rgb(26, 26, 26), view.meter());
        drawText(hdc, view.tempoRect().x + 12, view.tempoRect().y + 12, rgb(26, 26, 26), std::to_string(view.bpm()) + " BPM");
        drawText(hdc, view.titleRect().x + 12, view.titleRect().y + 12, rgb(26, 26, 26), view.title());
        drawText(hdc, view.composerRect().x + 12, view.composerRect().y + 12, rgb(26, 26, 26), view.composer());
    }

    fillRoundedRect(hdc, view.compatibilityToggleRect(), view.compatibilityMode() ? rgb(37, 99, 235) : rgb(243, 243, 243), 6);
    fillRoundedRect(hdc, view.livePreviewToggleRect(), view.livePreviewEnabled() ? rgb(37, 99, 235) : rgb(243, 243, 243), 6);
    drawText(hdc, view.compatibilityToggleRect().x + 32, view.compatibilityToggleRect().y + 2, rgb(26, 26, 26), "Modo Compatibilidade");
    drawText(hdc, view.livePreviewToggleRect().x + 32, view.livePreviewToggleRect().y + 2, rgb(26, 26, 26), "Preview ao vivo");

    fillRoundedRect(hdc, view.assistantBackRect(), rgb(243, 243, 243), 8);
    fillRoundedRect(hdc, view.assistantNextRect(), rgb(37, 99, 235), 8);
    drawText(hdc, view.assistantBackRect().x + 32, view.assistantBackRect().y + 10, rgb(26, 26, 26), "Voltar");
    drawText(hdc, view.assistantNextRect().x + 34, view.assistantNextRect().y + 10, rgb(245, 245, 245), "Proximo");
}

void drawUi(HDC hdc, const notascore::ui::MainWindow& view) {
    drawMainHome(hdc, view);
    if (view.wizardOpen()) {
        drawWizardPanel(hdc, view);
    }
    drawText(hdc, 20, view.height() - 30, rgb(80, 80, 80), "Status: " + view.statusText());
}

LRESULT CALLBACK windowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    auto* view = reinterpret_cast<notascore::ui::MainWindow*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

    switch (msg) {
    case WM_NCCREATE: {
        auto* createStruct = reinterpret_cast<CREATESTRUCT*>(lParam);
        SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(createStruct->lpCreateParams));
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    case WM_SIZE:
        if (view != nullptr) {
            view->resize(LOWORD(lParam), HIWORD(lParam));
            InvalidateRect(hwnd, nullptr, TRUE);
        }
        return 0;
    case WM_LBUTTONDOWN:
        if (view != nullptr) {
            const auto x = static_cast<int>(static_cast<short>(LOWORD(lParam)));
            const auto y = static_cast<int>(static_cast<short>(HIWORD(lParam)));
            view->onClick(x, y);
            InvalidateRect(hwnd, nullptr, TRUE);
        }
        return 0;
    case WM_PAINT: {
        if (view == nullptr) {
            return DefWindowProc(hwnd, msg, wParam, lParam);
        }
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        drawUi(hdc, *view);
        EndPaint(hwnd, &ps);
        return 0;
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
}

} // namespace

#elif defined(__linux__)

#include <X11/Xlib.h>
#include <X11/Xutil.h>

namespace {

unsigned long rgb(unsigned char r, unsigned char g, unsigned char b) {
    return (static_cast<unsigned long>(r) << 16U) | (static_cast<unsigned long>(g) << 8U) | static_cast<unsigned long>(b);
}

void drawRect(Display* d, Window w, GC gc, const notascore::ui::Rect& rect, unsigned long color) {
    XSetForeground(d, gc, color);
    XFillRectangle(d, w, gc, rect.x, rect.y, static_cast<unsigned int>(rect.width), static_cast<unsigned int>(rect.height));
}

void drawText(Display* d, Window w, GC gc, int x, int y, unsigned long color, const std::string& text) {
    XSetForeground(d, gc, color);
    XDrawString(d, w, gc, x, y, text.c_str(), static_cast<int>(text.size()));
}

void drawMainHome(Display* d, Window w, GC gc, const notascore::ui::MainWindow& view) {
    const auto width = view.width();
    const auto height = view.height();

    drawRect(d, w, gc, {0, 0, width, height}, rgb(243, 243, 243));
    drawRect(d, w, gc, {0, 0, width, 54}, rgb(255, 255, 255));
    drawRect(d, w, gc, {0, 54, 210, height - 54}, rgb(255, 255, 255));
    drawRect(d, w, gc, {0, 53, width, 1}, rgb(224, 224, 224));
    drawRect(d, w, gc, {209, 54, 1, height - 54}, rgb(224, 224, 224));

    drawText(d, w, gc, 20, 30, rgb(26, 26, 26), "NotaScore");
    drawText(d, w, gc, 160, 30, rgb(26, 26, 26), "Arquivo  Editar  Preferencias  Ajuda");
    drawText(d, w, gc, 24, 90, rgb(26, 26, 26), "Home");

    drawRect(d, w, gc, view.newScoreCardRect(), rgb(37, 99, 235));
    drawText(d, w, gc, view.newScoreCardRect().x + 56, view.newScoreCardRect().y + 94, rgb(245, 245, 245), "Nova Partitura");

    drawRect(d, w, gc, view.openProjectRect(), rgb(255, 255, 255));
    drawRect(d, w, gc, view.importMidiRect(), rgb(255, 255, 255));
    drawRect(d, w, gc, view.importMusicXmlRect(), rgb(255, 255, 255));
    drawText(d, w, gc, view.openProjectRect().x + 12, view.openProjectRect().y + 24, rgb(26, 26, 26), "Abrir Projeto");
    drawText(d, w, gc, view.importMidiRect().x + 12, view.importMidiRect().y + 24, rgb(26, 26, 26), "Importar MIDI");
    drawText(d, w, gc, view.importMusicXmlRect().x + 12, view.importMusicXmlRect().y + 24, rgb(26, 26, 26), "Importar MusicXML");

    drawText(d, w, gc, 560, 106, rgb(26, 26, 26), "Projetos Recentes");
    int y = 140;
    for (const auto& item : view.recentProjects()) {
        drawRect(d, w, gc, {560, y, 290, 28}, rgb(255, 255, 255));
        drawText(d, w, gc, 574, y + 19, rgb(26, 26, 26), item);
        y += 36;
        if (y > 300) {
            break;
        }
    }
}

void drawWizardPanel(Display* d, Window w, GC gc, const notascore::ui::MainWindow& view) {
    const auto panel = view.assistantPanelRect();
    drawRect(d, w, gc, panel, rgb(255, 255, 255));
    drawRect(d, w, gc, {panel.x, 0, 1, panel.height}, rgb(224, 224, 224));

    drawText(d, w, gc, panel.x + 24, 30, rgb(26, 26, 26), "Nova Partitura");

    if (view.wizardStep() == notascore::ui::WizardStep::Instruments) {
        drawText(d, w, gc, panel.x + 24, 54, rgb(80, 80, 80), "1. Instrumentos");
        drawText(d, w, gc, panel.x + 24, 86, rgb(26, 26, 26), "Biblioteca");
        drawText(d, w, gc, panel.x + 216, 86, rgb(26, 26, 26), "Selecionados");

        const std::size_t visible = std::min<std::size_t>(6, view.instrumentLibrary().size());
        for (std::size_t i = 0; i < visible; ++i) {
            const int y = 110 + static_cast<int>(i) * 34;
            const auto& inst = view.instrumentLibrary()[i];
            drawText(d, w, gc, panel.x + 24, y, rgb(26, 26, 26), inst.name);
            drawText(d, w, gc, panel.x + 90, y, rgb(110, 110, 110), inst.range);
            drawRect(d, w, gc, view.instrumentAddRect(i), rgb(229, 240, 255));
            drawText(d, w, gc, view.instrumentAddRect(i).x + 6, view.instrumentAddRect(i).y + 14, rgb(37, 99, 235), "+");
        }

        for (std::size_t i = 0; i < view.selectedInstruments().size(); ++i) {
            const int y = 110 + static_cast<int>(i) * 30;
            drawText(d, w, gc, panel.x + 216, y, rgb(26, 26, 26), view.selectedInstruments()[i].name);
            drawRect(d, w, gc, view.selectedInstrumentRemoveRect(i), rgb(255, 243, 243));
            drawText(d, w, gc, view.selectedInstrumentRemoveRect(i).x + 4, view.selectedInstrumentRemoveRect(i).y + 12, rgb(180, 40, 40), "x");
        }
    } else {
        drawText(d, w, gc, panel.x + 24, 54, rgb(80, 80, 80), "2. Configuracoes");
        drawRect(d, w, gc, view.keySignatureRect(), rgb(243, 243, 243));
        drawRect(d, w, gc, view.meterRect(), rgb(243, 243, 243));
        drawRect(d, w, gc, view.tempoRect(), rgb(243, 243, 243));
        drawRect(d, w, gc, view.titleRect(), rgb(243, 243, 243));
        drawRect(d, w, gc, view.composerRect(), rgb(243, 243, 243));
        drawText(d, w, gc, view.keySignatureRect().x + 12, view.keySignatureRect().y + 24, rgb(26, 26, 26), view.keySignature());
        drawText(d, w, gc, view.meterRect().x + 12, view.meterRect().y + 24, rgb(26, 26, 26), view.meter());
        drawText(d, w, gc, view.tempoRect().x + 12, view.tempoRect().y + 24, rgb(26, 26, 26), std::to_string(view.bpm()) + " BPM");
        drawText(d, w, gc, view.titleRect().x + 12, view.titleRect().y + 24, rgb(26, 26, 26), view.title());
        drawText(d, w, gc, view.composerRect().x + 12, view.composerRect().y + 24, rgb(26, 26, 26), view.composer());
    }

    drawRect(d, w, gc, view.compatibilityToggleRect(), view.compatibilityMode() ? rgb(37, 99, 235) : rgb(243, 243, 243));
    drawRect(d, w, gc, view.livePreviewToggleRect(), view.livePreviewEnabled() ? rgb(37, 99, 235) : rgb(243, 243, 243));
    drawText(d, w, gc, view.compatibilityToggleRect().x + 32, view.compatibilityToggleRect().y + 14, rgb(26, 26, 26), "Modo Compatibilidade");
    drawText(d, w, gc, view.livePreviewToggleRect().x + 32, view.livePreviewToggleRect().y + 14, rgb(26, 26, 26), "Preview ao vivo");

    drawRect(d, w, gc, view.assistantBackRect(), rgb(243, 243, 243));
    drawRect(d, w, gc, view.assistantNextRect(), rgb(37, 99, 235));
    drawText(d, w, gc, view.assistantBackRect().x + 30, view.assistantBackRect().y + 22, rgb(26, 26, 26), "Voltar");
    drawText(d, w, gc, view.assistantNextRect().x + 30, view.assistantNextRect().y + 22, rgb(245, 245, 245), "Proximo");
}

void drawUi(Display* d, Window w, GC gc, const notascore::ui::MainWindow& view) {
    drawMainHome(d, w, gc, view);
    if (view.wizardOpen()) {
        drawWizardPanel(d, w, gc, view);
    }
    drawText(d, w, gc, 20, view.height() - 20, rgb(80, 80, 80), "Status: " + view.statusText());
}

} // namespace
#endif

namespace notascore::platform {

NativeWindow::NativeWindow(notascore::ui::MainWindow& view)
    : m_view(view) {}

int NativeWindow::run() {
#if defined(_WIN32)
    const char* className = "NotaScoreWindowClass";
    WNDCLASSA wc {};
    wc.lpfnWndProc = windowProc;
    wc.hInstance = GetModuleHandle(nullptr);
    wc.lpszClassName = className;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);

    if (RegisterClassA(&wc) == 0) {
        return 1;
    }

    HWND hwnd = CreateWindowExA(
        0,
        className,
        "NotaScore",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        m_view.width(),
        m_view.height(),
        nullptr,
        nullptr,
        GetModuleHandle(nullptr),
        &m_view);

    if (hwnd == nullptr) {
        return 1;
    }

    ShowWindow(hwnd, SW_SHOWDEFAULT);
    UpdateWindow(hwnd);

    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return static_cast<int>(msg.wParam);
#elif defined(__linux__)
    Display* display = XOpenDisplay(nullptr);
    if (!display) {
        std::cerr << "X11 display not available.\n";
        return 1;
    }

    const int screen = DefaultScreen(display);
    Window window = XCreateSimpleWindow(display,
        RootWindow(display, screen),
        0,
        0,
        static_cast<unsigned int>(m_view.width()),
        static_cast<unsigned int>(m_view.height()),
        0,
        BlackPixel(display, screen),
        rgb(243, 243, 243));

    XSelectInput(display, window, ExposureMask | KeyPressMask | ButtonPressMask | StructureNotifyMask);
    XStoreName(display, window, "NotaScore");
    XMapWindow(display, window);

    GC gc = XCreateGC(display, window, 0, nullptr);

    bool running = true;
    while (running) {
        XEvent event;
        XNextEvent(display, &event);

        if (event.type == Expose) {
            drawUi(display, window, gc, m_view);
        } else if (event.type == ConfigureNotify) {
            m_view.resize(event.xconfigure.width, event.xconfigure.height);
            drawUi(display, window, gc, m_view);
        } else if (event.type == ButtonPress) {
            m_view.onClick(event.xbutton.x, event.xbutton.y);
            drawUi(display, window, gc, m_view);
        } else if (event.type == KeyPress) {
            running = false;
        }
    }

    XFreeGC(display, gc);
    XDestroyWindow(display, window);
    XCloseDisplay(display);
    return 0;
#else
    std::cerr << "Native GUI backend is currently implemented for Windows and Linux/X11.\n";
    return 1;
#endif
}

} // namespace notascore::platform
