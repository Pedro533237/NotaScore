#pragma once

#include <QOpenGLWidget>

namespace notascore::ui {

class PreviewWidget : public QOpenGLWidget {
    Q_OBJECT
public:
    explicit PreviewWidget(QWidget* parent = nullptr);
protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
};

} // namespace notascore::ui
