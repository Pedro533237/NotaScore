#pragma once

#include "notascore/ui/Theme.hpp"

#include <QWidget>
#include <QPainterPath>
#include <memory>

namespace notascore::ui {

/**
 * Modern Card Widget - Windows 11 inspired with rounded corners and shadow
 */
class ModernCard : public QWidget {
    Q_OBJECT
public:
    explicit ModernCard(QWidget* parent = nullptr);

    void setTitle(const QString& title);
    void setSubtitle(const QString& subtitle);
    void setIcon(const QIcon& icon);
    void setHoverable(bool hoverable = true) { m_hoverable = hoverable; }
    
    [[nodiscard]] bool isHovered() const noexcept { return m_isHovered; }

signals:
    void clicked();

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void enterEvent(QEnterEvent* event) override;
    void leaveEvent(QEvent* event) override;

private:
    void drawShadow(QPainter& painter, const QRect& bounds);
    void drawBackground(QPainter& painter, const QRect& bounds);
    void drawContent(QPainter& painter, const QRect& bounds);

    QString m_title;
    QString m_subtitle;
    QIcon m_icon;
    bool m_hoverable {true};
    bool m_isHovered {false};
};

/**
 * Modern Button - Custom rendered with smooth hover effects
 */
class ModernButton : public QWidget {
    Q_OBJECT
public:
    enum class Style {
        Primary,    // Blue accent
        Secondary,  // Outlined
        Subtle      // Text only
    };

    explicit ModernButton(const QString& text, Style style = Style::Primary, QWidget* parent = nullptr);

    void setText(const QString& text) { m_text = text; update(); }
    void setIcon(const QIcon& icon) { m_icon = icon; update(); }
    void setEnabled(bool enabled) { m_enabled = enabled; update(); }
    
    [[nodiscard]] bool isEnabled() const noexcept { return m_enabled; }
    [[nodiscard]] bool isHovered() const noexcept { return m_isHovered; }
    [[nodiscard]] bool isPressed() const noexcept { return m_isPressed; }

    QSize sizeHint() const override;

signals:
    void clicked();

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void enterEvent(QEnterEvent* event) override;
    void leaveEvent(QEvent* event) override;

private:
    QString m_text;
    QIcon m_icon;
    Style m_style;
    bool m_enabled {true};
    bool m_isHovered {false};
    bool m_isPressed {false};
};

/**
 * Modern Input Field - Custom styled line edit
 */
class ModernLineEdit : public QWidget {
    Q_OBJECT
public:
    explicit ModernLineEdit(QWidget* parent = nullptr);

    void setText(const QString& text);
    void setPlaceholder(const QString& placeholder);
    void setLabel(const QString& label);
    
    [[nodiscard]] QString text() const;
    [[nodiscard]] QString label() const { return m_label; }

    QSize sizeHint() const override;

signals:
    void textChanged(const QString& text);
    void returnPressed();

protected:
    void paintEvent(QPaintEvent* event) override;
    void focusInEvent(QFocusEvent* event) override;
    void focusOutEvent(QFocusEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;

private:
    QString m_text;
    QString m_placeholder;
    QString m_label;
    bool m_focused {false};
};

/**
 * Modern Slider - Custom rendered slider
 */
class ModernSlider : public QWidget {
    Q_OBJECT
public:
    explicit ModernSlider(QWidget* parent = nullptr);

    void setRange(int min, int max);
    void setValue(int value);
    void setLabel(const QString& label);
    
    [[nodiscard]] int value() const noexcept { return m_value; }
    [[nodiscard]] int minimum() const noexcept { return m_min; }
    [[nodiscard]] int maximum() const noexcept { return m_max; }

    QSize sizeHint() const override;

signals:
    void valueChanged(int value);

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

private:
    int m_value {0};
    int m_min {0};
    int m_max {100};
    QString m_label;
    bool m_dragging {false};
};

/**
 * Performance Mode Toggle - Checkbox-like widget for performance settings
 */
class PerformanceToggle : public QWidget {
    Q_OBJECT
public:
    explicit PerformanceToggle(const QString& label, QWidget* parent = nullptr);

    void setChecked(bool checked) { m_checked = checked; update(); }
    void setDescription(const QString& desc) { m_description = desc; update(); }
    
    [[nodiscard]] bool isChecked() const noexcept { return m_checked; }

    QSize sizeHint() const override;

signals:
    void toggled(bool checked);

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void enterEvent(QEnterEvent* event) override;
    void leaveEvent(QEvent* event) override;

private:
    QString m_label;
    QString m_description;
    bool m_checked {false};
    bool m_hovered {false};
};

} // namespace notascore::ui
