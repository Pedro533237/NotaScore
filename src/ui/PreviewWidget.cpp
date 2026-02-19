#ifdef NOTASCORE_ENABLE_QT

#include "notascore/ui/PreviewWidget.hpp"

#include <QPainter>

namespace notascore::ui {

PreviewWidget::PreviewWidget(QWidget* parent)
    : QOpenGLWidget(parent) {
    setMinimumSize(160, 120);
}

void PreviewWidget::initializeGL() {
    // Lightweight initialization; keep GL calls minimal
}

void PreviewWidget::resizeGL(int w, int h) {
    Q_UNUSED(w)
    Q_UNUSED(h)
}

void PreviewWidget::paintGL() {
    QPainter p(this);
    p.fillRect(rect(), QColor("#F3F3F3"));
    p.setPen(QColor("#1A1A1A"));
    p.drawText(rect().adjusted(8, 8, -8, -8), Qt::AlignLeft | Qt::AlignTop, "Preview");
}

} // namespace notascore::ui

#endif // NOTASCORE_ENABLE_QT
