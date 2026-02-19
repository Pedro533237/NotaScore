#pragma once

#include "notascore/ui/MainWindow.hpp"

namespace notascore::platform {

class NativeWindow {
public:
    explicit NativeWindow(notascore::ui::MainWindow& view);
    int run();

private:
    notascore::ui::MainWindow& m_view;
};

} // namespace notascore::platform
