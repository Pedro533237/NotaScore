#include "notascore/ui/Theme.hpp"

#include <format>

namespace notascore::ui {

Theme g_theme{ThemeMode::Light};

std::string Color::toHex() const noexcept {
    return std::format("#{:02X}{:02X}{:02X}", r, g, b);
}

Theme::Theme(ThemeMode m) : mode(m) {
    setMode(m);
}

void Theme::setMode(ThemeMode m) noexcept {
    mode = m;
    
    if (m == ThemeMode::Dark) {
        backgroundMain = DarkThemePalette::BackgroundMain;
        panelBackground = DarkThemePalette::PanelBackground;
        accentColor = DarkThemePalette::AccentColor;
        hoverOverlay = DarkThemePalette::HoverOverlay;
        textPrimary = DarkThemePalette::TextPrimary;
        textSecondary = DarkThemePalette::TextSecondary;
        divider = DarkThemePalette::Divider;
        shadowColor = DarkThemePalette::ShadowColor;
    } else {
        backgroundMain = LightThemePalette::BackgroundMain;
        panelBackground = LightThemePalette::PanelBackground;
        accentColor = LightThemePalette::AccentColor;
        hoverOverlay = LightThemePalette::HoverOverlay;
        textPrimary = LightThemePalette::TextPrimary;
        textSecondary = LightThemePalette::TextSecondary;
        divider = LightThemePalette::Divider;
        shadowColor = LightThemePalette::ShadowColor;
    }
}

} // namespace notascore::ui
