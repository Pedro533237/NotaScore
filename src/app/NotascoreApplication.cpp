#include "notascore/app/NotascoreApplication.hpp"

#include <QApplication>
#include <QDebug>
#include <QSysInfo>

namespace notascore {

NotascoreApplication::NotascoreApplication(int argc, char* argv[])
    : m_qapp(std::make_unique<QApplication>(argc, argv)) {
    
    // Configurar aplicação Qt
    m_qapp->setApplicationName("NotaScore");
    m_qapp->setApplicationVersion("0.1.0");
    m_qapp->setApplicationAuthor("NotaScore Team");
    
    // Inicializar sistema de temas
    initializeTheme();
    
    // Inicializar sistema de performance
    initializePerformance();
    
    // Criar view model
    core::PerformanceSettings settings;
    settings.cpuModeOnly = false;
    settings.gpuAccelerationOptional = true;
    settings.disableAnimations = false;
    settings.disableShadows = false;
    settings.disableSmoothZoom = false;
    settings.lowMemoryMode = false;
    
    m_viewModel = std::make_unique<ui::MainWindow>(1200, 800, settings);
    
    // Criar janela principal
    m_mainWindow = std::make_unique<ui::QtMainWindow>(*m_viewModel);
    
    setupSignalHandlers();
}

NotascoreApplication::~NotascoreApplication() = default;

int NotascoreApplication::run() {
    m_mainWindow->show();
    return m_qapp->exec();
}

void NotascoreApplication::setTheme(ui::ThemeMode mode) {
    ui::g_theme.setMode(mode);
    
    // Forçar repaint de todos os widgets
    for (auto* widget : m_qapp->allWidgets()) {
        widget->update();
    }
}

void NotascoreApplication::toggleTheme() {
    setTheme(ui::g_theme.isDark() ? ui::ThemeMode::Light : ui::ThemeMode::Dark);
}

void NotascoreApplication::setPerformanceMode(core::PerformanceProfile::Mode mode) {
    if (!m_perfProfile) {
        return;
    }
    
    m_perfProfile->setMode(mode);
    
    // Aplicar configurações ao view model
    if (m_viewModel) {
        // Configurações será aplicadas dinamicamente
    }
    
    qDebug() << "Performance mode set to:" << static_cast<int>(mode);
}

void NotascoreApplication::autodetectPerformanceMode() {
    // Detectar GPU
    QString gpuRenderer = QString::fromUtf8(reinterpret_cast<const char*>(
        glGetString(GL_RENDERER)
    ));
    
    qDebug() << "GPU:" << gpuRenderer;
    
    // Detectar RAM disponível
    qint64 totalMemory = 0;
    #ifdef Q_OS_WIN
        MEMORYSTATUSEX stat;
        stat.dwLength = sizeof(stat);
        GlobalMemoryStatusEx(&stat);
        totalMemory = stat.ullTotalPhys;
    #elif defined(Q_OS_LINUX)
        // Ler de /proc/meminfo
        #include <unistd.h>
        totalMemory = sysconf(_SC_PHYS_PAGES) * sysconf(_SC_PAGE_SIZE);
    #elif defined(Q_OS_MAC)
        int mib[2] = { CTL_HW, HW_MEMSIZE };
        size_t length = sizeof(totalMemory);
        sysctl(mib, 2, &totalMemory, &length, NULL, 0);
    #endif
    
    qDebug() << "Total memory (GB):" << (totalMemory / 1e9);
    
    // Decidir modo baseado em recursos
    if (totalMemory < 4e9 || gpuRenderer.contains("Intel", Qt::CaseInsensitive)) {
        qDebug() << "Detected weak hardware - enabling compatibility mode";
        setPerformanceMode(core::PerformanceProfile::Mode::LowEnd);
    } else if (totalMemory < 8e9) {
        setPerformanceMode(core::PerformanceProfile::Mode::Balanced);
    } else {
        setPerformanceMode(core::PerformanceProfile::Mode::HighPerformance);
    }
}

void NotascoreApplication::createNewScore() {
    // Triggerizar wizard na view model
    auto r = m_viewModel->newScoreCardRect();
    m_viewModel->onClick(r.x + 8, r.y + 8);
    
    // Refresh UI
    if (m_mainWindow) {
        m_mainWindow->refresh();
    }
}

void NotascoreApplication::openScore(const std::string& filepath) {
    // TODO: Implementar carregamento de score
    qDebug() << "Opening score:" << QString::fromStdString(filepath);
}

void NotascoreApplication::saveScore(const std::string& filepath) {
    // TODO: Implementar salvamento de score
    qDebug() << "Saving score to:" << QString::fromStdString(filepath);
}

void NotascoreApplication::initializeTheme() {
    // Detectar preferência do sistema
    bool preferDarkMode = false;
    
    #ifdef Q_OS_WIN
        // Windows: Ler do registro
        // HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Themes\Personalize
        // AppsUseLightTheme = 0 (dark) or 1 (light)
    #elif defined(Q_OS_LINUX)
        // Linux: Verificar variável de ambiente
        const char* xdgTheme = std::getenv("XDG_CURRENT_DESKTOP");
        preferDarkMode = (xdgTheme != nullptr);
    #elif defined(Q_OS_MAC)
        // macOS: Usar AppleInterfaceStyle
        // preferDarkMode = checkSystemAppearance();
    #endif
    
    ui::ThemeMode initialMode = preferDarkMode ? ui::ThemeMode::Dark : ui::ThemeMode::Light;
    ui::g_theme.setMode(initialMode);
    
    qDebug() << "Theme initialized:" << (preferDarkMode ? "Dark" : "Light");
}

void NotascoreApplication::initializePerformance() {
    m_perfProfile = std::make_unique<core::PerformanceProfile>();
    
    // Auto-detectar modo se nenhum for especificado
    // autodetectPerformanceMode();  // Chamado manualmente se necessário
}

void NotascoreApplication::setupSignalHandlers() {
    // Conectar menu file -> new score
    // connect(m_mainWindow->ui()->actionNewScore, ...)
    
    // Conectar keyboard shortcuts
    // Ctrl+N -> New Score
    // Ctrl+O -> Open
    // Ctrl+S -> Save
}

} // namespace notascore
