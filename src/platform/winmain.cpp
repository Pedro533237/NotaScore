#if defined(_WIN32)

#include "notascore/app/Application.hpp"

#include <windows.h>

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    notascore::app::Application app;
    return app.run();
}

#endif
