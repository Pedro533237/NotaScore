#pragma once

#ifdef NOTASCORE_ENABLE_QT

#include "notascore/ui/MainWindow.hpp"
#include "notascore/ui/InstrumentModel.hpp"
#include "notascore/ui/PreviewWidget.hpp"

#include <QMainWindow>

namespace notascore::ui {

class QtMainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit QtMainWindow(notascore::ui::MainWindow& view, QWidget* parent = nullptr);

    void refresh();

private:
    void loadModernStylesheet();

    notascore::ui::MainWindow& m_view;
    InstrumentModel* m_instrumentModel;
    PreviewWidget* m_preview;
    QListWidget* m_selectedList;
    QListWidget* m_recentList;
};

} // namespace notascore::ui

#endif // NOTASCORE_ENABLE_QT
