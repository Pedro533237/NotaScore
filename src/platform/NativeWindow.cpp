#include "notascore/platform/NativeWindow.hpp"

#include <iostream>
#include <string>

#if defined(__linux__)
#include <X11/Xlib.h>
#include <X11/Xutil.h>

namespace {

unsigned long rgb(unsigned char r, unsigned char g, unsigned char b) {
    return (static_cast<unsigned long>(r) << 16U) | (static_cast<unsigned long>(g) << 8U) | static_cast<unsigned long>(b);
}

void drawRect(Display* d, Window w, GC gc, int x, int y, int width, int height, unsigned long color, bool fill = true) {
    XSetForeground(d, gc, color);
    if (fill) {
        XFillRectangle(d, w, gc, x, y, static_cast<unsigned int>(width), static_cast<unsigned int>(height));
    } else {
        XDrawRectangle(d, w, gc, x, y, static_cast<unsigned int>(width), static_cast<unsigned int>(height));
    }
}

void drawText(Display* d, Window w, GC gc, int x, int y, unsigned long color, const char* text) {
    XSetForeground(d, gc, color);
    XDrawString(d, w, gc, x, y, text, static_cast<int>(std::char_traits<char>::length(text)));
}

void drawUi(Display* d, Window w, GC gc, const notascore::ui::MainWindow& view) {
    const auto width = view.width();
    const auto height = view.height();

    drawRect(d, w, gc, 0, 0, width, height, rgb(43, 45, 50));
    drawRect(d, w, gc, 0, 0, width, 38, rgb(35, 36, 40));
    drawRect(d, w, gc, 0, 38, 220, height - 38, rgb(38, 39, 44));
    drawRect(d, w, gc, 0, 118, 220, 44, rgb(26, 71, 106));

    drawText(d, w, gc, 16, 25, rgb(230, 230, 230), "Home   Score   Publish   DevTools");
    drawText(d, w, gc, 18, 96, rgb(220, 220, 220), "My account");
    drawText(d, w, gc, 18, 145, rgb(235, 235, 235), "Scores");
    drawText(d, w, gc, 240, 90, rgb(240, 240, 240), "Scores");
    drawText(d, w, gc, 240, 130, rgb(220, 220, 220), "New & recent");

    const auto card = view.newScoreCardRect();
    drawRect(d, w, gc, card.x, card.y, card.width, card.height, rgb(218, 218, 218));
    drawText(d, w, gc, card.x + 78, card.y + 118, rgb(20, 20, 20), "+");
    drawText(d, w, gc, card.x + 44, card.y + card.height + 24, rgb(220, 220, 220), "New score");

    drawRect(d, w, gc, width - 280, 52, 220, 32, rgb(28, 30, 35));
    drawText(d, w, gc, width - 264, 73, rgb(130, 136, 145), "Search");

    drawRect(d, w, gc, 0, height - 74, width, 74, rgb(48, 50, 57));
    drawText(d, w, gc, width - 300, height - 28, rgb(235, 235, 235), "New");
    drawText(d, w, gc, width - 180, height - 28, rgb(235, 235, 235), "Open other...");

    const auto status = "Status: " + view.statusText();
    drawText(d, w, gc, 20, height - 28, rgb(220, 220, 220), status.c_str());
}

} // namespace
#endif

namespace notascore::platform {

NativeWindow::NativeWindow(notascore::ui::MainWindow& view)
    : m_view(view) {}

int NativeWindow::run() {
#if defined(__linux__)
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
        rgb(43, 45, 50));

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
    std::cerr << "Native GUI backend is currently implemented for Linux/X11 in this environment.\n";
    return 1;
#endif
}

} // namespace notascore::platform
