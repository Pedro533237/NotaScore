#include "notascore/ui/ModernWidgets.hpp"

#include <QPainter>
#include <QPainterPath>
#include <QMouseEvent>
#include <QIcon>
#include <QFontMetrics>
#include <QApplication>
#include <QWidget>

namespace notascore::ui {

// Helper: convert internal Color -> QColor without exposing Qt in public headers
static inline QColor toQColor(const Color &c) {
    return QColor(c.r, c.g, c.b, c.a);
}


// ============================================================================
// ModernCard Implementation
// ============================================================================

ModernCard::ModernCard(QWidget* parent)
    : QWidget(parent) {
    setMinimumSize(260, 170);
    setMaximumSize(400, 250);
    setCursor(m_hoverable ? Qt::PointingHandCursor : Qt::ArrowCursor);
}

void ModernCard::setTitle(const QString& title) {
    m_title = title;
    update();
}

void ModernCard::setSubtitle(const QString& subtitle) {
    m_subtitle = subtitle;
    update();
}

void ModernCard::setIcon(const QIcon& icon) {
    m_icon = icon;
    update();
}

void ModernCard::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event);
    
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);

    QRect bounds = rect();
    
    // Draw shadow
    drawShadow(painter, bounds);
    
    // Draw background with smooth transition
    drawBackground(painter, bounds);
    
    // Draw content
    drawContent(painter, bounds);
}

void ModernCard::drawShadow(QPainter& painter, const QRect& bounds) {
    if (!g_theme.isDark()) {
        painter.fillRect(bounds.translated(0, 2), QColor(0, 0, 0, 8));
        painter.fillRect(bounds.translated(0, 4), QColor(0, 0, 0, 4));
    }
}

void ModernCard::drawBackground(QPainter& painter, const QRect& bounds) {
    QPainterPath path;
    path.addRoundedRect(bounds.adjusted(0, 0, -1, -1), 
                       Theme::CornerRadius, Theme::CornerRadius);
    
    QColor bgColor = m_isHovered ? toQColor(g_theme.hoverOverlay) : toQColor(g_theme.panelBackground);
    painter.fillPath(path, bgColor);
    
    painter.strokePath(path, QPen(toQColor(g_theme.divider), 1));
}

void ModernCard::drawContent(QPainter& painter, const QRect& bounds) {
    // Draw icon if available
    if (!m_icon.isNull()) {
        QPixmap pixmap = m_icon.pixmap(64, 64);
        painter.drawPixmap(bounds.left() + 20, bounds.top() + 20, pixmap);
    }
    
    // Draw title
    QFont titleFont;
    titleFont.setPointSize(14);
    titleFont.setWeight(QFont::DemiBold);
    painter.setFont(titleFont);
    painter.setPen(toQColor(g_theme.textPrimary));
    
    QRect titleRect(bounds.left() + 20, bounds.top() + 100, bounds.width() - 40, 30);
    painter.drawText(titleRect, Qt::AlignLeft | Qt::AlignVCenter, m_title);
    
    // Draw subtitle if available
    if (!m_subtitle.isEmpty()) {
        QFont subtitleFont;
        subtitleFont.setPointSize(10);
        painter.setFont(subtitleFont);
        painter.setPen(toQColor(g_theme.textSecondary));
        
        QRect subtitleRect(bounds.left() + 20, bounds.top() + 130, bounds.width() - 40, 20);
        painter.drawText(subtitleRect, Qt::AlignLeft | Qt::AlignVCenter, m_subtitle);
    }
}

void ModernCard::mousePressEvent(QMouseEvent* event) {
    Q_UNUSED(event);
    if (m_hoverable) {
        emit clicked();
    }
}

void ModernCard::enterEvent(QEnterEvent* event) {
    Q_UNUSED(event);
    if (m_hoverable) {
        m_isHovered = true;
        update();
    }
}

void ModernCard::leaveEvent(QEvent* event) {
    Q_UNUSED(event);
    m_isHovered = false;
    update();
}

// ============================================================================
// ModernButton Implementation
// ============================================================================

ModernButton::ModernButton(const QString& text, Style style, QWidget* parent)
    : QWidget(parent), m_text(text), m_style(style) {
    setMinimumSize(90, 36);
    setCursor(Qt::PointingHandCursor);
}

QSize ModernButton::sizeHint() const {
    QFontMetrics fm(font());
    int width = fm.horizontalAdvance(m_text) + 40;
    return QSize(qMax(width, 90), 36);
}

void ModernButton::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event);
    
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QRect bounds = rect().adjusted(0, 0, -1, -1);
    
    // Determine colors based on style and state
    QColor bgColor;
    QColor textColor;
    QColor borderColor;

    if (m_style == Style::Primary) {
        if (!m_enabled) {
            bgColor = QColor(203, 213, 225);      // Disabled gray
            textColor = QColor(148, 163, 184);
            borderColor = bgColor;
        } else if (m_isPressed) {
            bgColor = QColor(30, 64, 175);        // Dark blue
            textColor = Qt::white;
            borderColor = bgColor;
        } else if (m_isHovered) {
            bgColor = QColor(29, 78, 216);        // Medium blue
            textColor = Qt::white;
            borderColor = bgColor;
        } else {
            bgColor = QColor(37, 99, 235);        // Primary blue
            textColor = Qt::white;
            borderColor = bgColor;
        }
    } else if (m_style == Style::Secondary) {
        bgColor = m_isHovered ? toQColor(g_theme.hoverOverlay) : QColor(Qt::transparent);
        textColor = m_enabled ? toQColor(g_theme.accentColor) : toQColor(g_theme.textSecondary);
        borderColor = m_isHovered ? toQColor(g_theme.accentColor) : toQColor(g_theme.divider);
    } else { // Subtle
        bgColor = QColor(Qt::transparent);
        textColor = m_enabled ? toQColor(g_theme.textPrimary) : toQColor(g_theme.textSecondary);
        borderColor = QColor(Qt::transparent);
    }

    // Draw background
    QPainterPath path;
    path.addRoundedRect(bounds, 8, 8);
    
    painter.fillPath(path, bgColor);
    if (m_style != Style::Subtle) {
        painter.strokePath(path, QPen(borderColor, 1));
    }

    // Draw text
    QFont buttonFont;
    buttonFont.setPointSize(11);
    buttonFont.setWeight(QFont::Medium);
    painter.setFont(buttonFont);
    painter.setPen(textColor);
    
    painter.drawText(bounds, Qt::AlignCenter, m_text);
}

void ModernButton::mousePressEvent(QMouseEvent* event) {
    Q_UNUSED(event);
    if (m_enabled) {
        m_isPressed = true;
        update();
    }
}

void ModernButton::mouseReleaseEvent(QMouseEvent* event) {
    Q_UNUSED(event);
    m_isPressed = false;
    if (m_enabled && rect().contains(event->pos())) {
        emit clicked();
    }
    update();
}

void ModernButton::enterEvent(QEnterEvent* event) {
    Q_UNUSED(event);
    if (m_enabled) {
        m_isHovered = true;
        update();
    }
}

void ModernButton::leaveEvent(QEvent* event) {
    Q_UNUSED(event);
    m_isHovered = false;
    m_isPressed = false;
    update();
}

// ============================================================================
// ModernLineEdit Implementation
// ============================================================================

ModernLineEdit::ModernLineEdit(QWidget* parent)
    : QWidget(parent) {
    setMinimumSize(200, 40);
}

void ModernLineEdit::setText(const QString& text) {
    m_text = text;
    update();
}

void ModernLineEdit::setPlaceholder(const QString& placeholder) {
    m_placeholder = placeholder;
    update();
}

void ModernLineEdit::setLabel(const QString& label) {
    m_label = label;
    update();
}

QString ModernLineEdit::text() const {
    return m_text;
}

QSize ModernLineEdit::sizeHint() const {
    return QSize(300, 50);
}

void ModernLineEdit::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event);
    
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Draw label if present
    if (!m_label.isEmpty()) {
        QFont labelFont;
        labelFont.setPointSize(11);
        labelFont.setWeight(QFont::Medium);
        painter.setFont(labelFont);
        painter.setPen(toQColor(g_theme.textPrimary));
        painter.drawText(QRect(0, 0, width(), 16), Qt::AlignLeft, m_label);
    }

    // Draw input field border and background
    QRect inputRect(0, m_label.isEmpty() ? 0 : 22, width(), 32);
    QPainterPath path;
    path.addRoundedRect(inputRect, 6, 6);
    
    painter.fillPath(path, toQColor(g_theme.panelBackground));
    
    QColor borderColor = m_focused ? toQColor(g_theme.accentColor) : toQColor(g_theme.divider);
    int borderWidth = m_focused ? 2 : 1;
    painter.strokePath(path, QPen(borderColor, borderWidth));

    // Draw text
    QFont textFont;
    textFont.setPointSize(11);
    painter.setFont(textFont);
    painter.setPen(toQColor(g_theme.textPrimary));
    
    QString displayText = m_text.isEmpty() ? m_placeholder : m_text;
    if (displayText == m_placeholder) {
        painter.setPen(toQColor(g_theme.textSecondary));
    }
    
    painter.drawText(inputRect.adjusted(8, 0, -8, 0), Qt::AlignVCenter, displayText);
}

void ModernLineEdit::focusInEvent(QFocusEvent* event) {
    Q_UNUSED(event);
    m_focused = true;
    update();
}

void ModernLineEdit::focusOutEvent(QFocusEvent* event) {
    Q_UNUSED(event);
    m_focused = false;
    update();
}

void ModernLineEdit::mousePressEvent(QMouseEvent* event) {
    Q_UNUSED(event);
    setFocus();
}

void ModernLineEdit::keyPressEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_Backspace && !m_text.isEmpty()) {
        m_text.chop(1);
        emit textChanged(m_text);
    } else if (event->key() == Qt::Key_Return) {
        emit returnPressed();
    } else {
        QString t = event->text();
        if (!t.isEmpty() && t.at(0).isPrint()) {
            m_text += t;
            emit textChanged(m_text);
        }
    }
    update();
}

// ============================================================================
// ModernSlider Implementation
// ============================================================================

ModernSlider::ModernSlider(QWidget* parent)
    : QWidget(parent) {
    setMinimumSize(200, 60);
}

void ModernSlider::setRange(int min, int max) {
    m_min = min;
    m_max = max;
    update();
}

void ModernSlider::setValue(int value) {
    m_value = qBound(m_min, value, m_max);
    emit valueChanged(m_value);
    update();
}

void ModernSlider::setLabel(const QString& label) {
    m_label = label;
    update();
}

QSize ModernSlider::sizeHint() const {
    return QSize(300, 60);
}

void ModernSlider::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event);
    
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Draw label
    if (!m_label.isEmpty()) {
        QFont labelFont;
        labelFont.setPointSize(11);
        labelFont.setWeight(QFont::Medium);
        painter.setFont(labelFont);
        painter.setPen(toQColor(g_theme.textPrimary));
        painter.drawText(QRect(0, 0, width(), 20), Qt::AlignLeft, m_label);
    }

    // Draw track
    QRect trackRect(10, 24, width() - 20, 4);
    painter.fillRect(trackRect, toQColor(g_theme.divider));

    // Draw filled portion
    if (m_max > m_min) {
        int filledWidth = (width() - 20) * (m_value - m_min) / (m_max - m_min);
        QRect filledRect(10, 24, filledWidth, 4);
        painter.fillRect(filledRect, toQColor(g_theme.accentColor));
    }

    // Draw handle
    if (m_max > m_min) {
        int handleX = 10 + (width() - 20) * (m_value - m_min) / (m_max - m_min) - 8;
        QRect handleRect(handleX, 16, 16, 20);
        
        painter.fillRect(handleRect, toQColor(g_theme.accentColor));
        painter.drawRoundedRect(handleRect, 4, 4);
    }

    // Draw value text
    QFont valueFont;
    valueFont.setPointSize(10);
    painter.setFont(valueFont);
    painter.setPen(toQColor(g_theme.textSecondary));
    painter.drawText(QRect(width() - 30, 24, 30, 20), Qt::AlignRight | Qt::AlignVCenter, 
                    QString::number(m_value));
}

void ModernSlider::mousePressEvent(QMouseEvent* event) {
    m_dragging = true;
    mouseMoveEvent(event);
}

void ModernSlider::mouseMoveEvent(QMouseEvent* event) {
    if (!m_dragging) return;
    
    if (m_max > m_min) {
        int newValue = m_min + (event->pos().x() - 10) * (m_max - m_min) / (width() - 20);
        setValue(newValue);
    }
}

void ModernSlider::mouseReleaseEvent(QMouseEvent* event) {
    Q_UNUSED(event);
    m_dragging = false;
}

// ============================================================================
// PerformanceToggle Implementation
// ============================================================================

PerformanceToggle::PerformanceToggle(const QString& label, QWidget* parent)
    : QWidget(parent), m_label(label) {
    setMinimumSize(300, 48);
    setCursor(Qt::PointingHandCursor);
}

QSize PerformanceToggle::sizeHint() const {
    return QSize(300, 48);
}

void PerformanceToggle::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event);
    
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QRect bounds = rect();

    // Draw background on hover
    if (m_hovered) {
        painter.fillRect(bounds, toQColor(g_theme.hoverOverlay));
    }

    // Draw checkbox
    QRect checkboxRect(10, bounds.center().y() - 10, 20, 20);
    painter.drawRoundedRect(checkboxRect, 4, 4);
    
    QColor borderColor = m_checked ? toQColor(g_theme.accentColor) : toQColor(g_theme.divider);
    painter.strokePath(QPainterPath(), QPen(borderColor, 1));

    if (m_checked) {
        painter.fillRect(checkboxRect, toQColor(g_theme.accentColor));
        // Draw checkmark
        painter.setPen(QPen(Qt::white, 2));
        painter.drawLine(checkboxRect.left() + 5, checkboxRect.center().y(),
                        checkboxRect.left() + 9, checkboxRect.bottom() - 5);
        painter.drawLine(checkboxRect.left() + 9, checkboxRect.bottom() - 5,
                        checkboxRect.right() - 5, checkboxRect.top() + 5);
    }

    // Draw label
    QFont labelFont;
    labelFont.setPointSize(11);
    labelFont.setWeight(QFont::Medium);
    painter.setFont(labelFont);
    painter.setPen(toQColor(g_theme.textPrimary));
    
    QRect labelRect(40, bounds.top(), bounds.width() - 50, bounds.height());
    painter.drawText(labelRect, Qt::AlignLeft | Qt::AlignVCenter, m_label);

    // Draw description if present
    if (!m_description.isEmpty()) {
        QFont descFont;
        descFont.setPointSize(9);
        painter.setFont(descFont);
        painter.setPen(toQColor(g_theme.textSecondary));
        
        QRect descRect(40, bounds.center().y() + 4, bounds.width() - 50, 14);
        painter.drawText(descRect, Qt::AlignLeft | Qt::AlignTop, m_description);
    }
}

void PerformanceToggle::mousePressEvent(QMouseEvent* event) {
    Q_UNUSED(event);
    m_checked = !m_checked;
    emit toggled(m_checked);
    update();
}

void PerformanceToggle::enterEvent(QEnterEvent* event) {
    Q_UNUSED(event);
    m_hovered = true;
    update();
}

void PerformanceToggle::leaveEvent(QEvent* event) {
    Q_UNUSED(event);
    m_hovered = false;
    update();
}

} // namespace notascore::ui
