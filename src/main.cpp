#if defined(NOTASCORE_ENABLE_QT)
#include "notascore/app/NotascoreApplication.hpp"

int main(int argc, char* argv[]) {
    // Usa a aplicação Qt moderna quando disponível
    notascore::NotascoreApplication app(argc, argv);
    app.autodetectPerformanceMode();
    app.createNewScore(); // abrir wizard na inicialização para demonstração
    return app.run();
}
#else
#include "notascore/app/Application.hpp"

int main() {
    notascore::app::Application app;
    return app.run();
}
#endif
