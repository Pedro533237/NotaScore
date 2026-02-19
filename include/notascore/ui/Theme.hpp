#pragma once

#include <cstdint>
#include <string>

namespace notascore::ui {

enum class ThemeMode {
    Light,
    Dark,
    Auto
};

// Color structure with RGBA components
struct Color {
    std::uint8_t r, g, b, a;

    [[nodiscard]] std::uint32_t toARGB() const noexcept {
        return (static_cast<std::uint32_t>(a) << 24) |
               (static_cast<std::uint32_t>(r) << 16) |
               (static_cast<std::uint32_t>(g) << 8) |
               (static_cast<std::uint32_t>(b));
    }

    [[nodiscard]] std::string toHex() const noexcept;
};

// Light theme palette (Windows 11 inspired)
struct LightThemePalette {
    static constexpr Color BackgroundMain{243, 243, 243, 255};      // #F3F3F3
    static constexpr Color PanelBackground{255, 255, 255, 255};    // #FFFFFF
    static constexpr Color AccentColor{37, 99, 235, 255};          // #2563EB
    static constexpr Color HoverOverlay{229, 240, 255, 255};       // #E5F0FF
    static constexpr Color TextPrimary{26, 26, 26, 255};           // #1A1A1A
    static constexpr Color TextSecondary{110, 110, 110, 255};      // #6E6E6E
    static constexpr Color Divider{224, 224, 224, 255};            // #E0E0E0
    static constexpr Color ShadowColor{0, 0, 0, 20};               // Soft shadow
};

// Dark theme palette (Windows 11 inspired)
struct DarkThemePalette {
    static constexpr Color BackgroundMain{31, 31, 31, 255};        // #1F1F1F
    static constexpr Color PanelBackground{42, 42, 42, 255};       // #2A2A2A
    static constexpr Color AccentColor{59, 130, 246, 255};         // #3B82F6
    static constexpr Color HoverOverlay{70, 100, 180, 255};        // Darker hover
    static constexpr Color TextPrimary{245, 245, 245, 255};        // #F5F5F5
    static constexpr Color TextSecondary{170, 170, 170, 255};      // #AAAAAA
    static constexpr Color Divider{50, 50, 50, 255};               // #323232
    static constexpr Color ShadowColor{0, 0, 0, 40};               // Stronger shadow
};

// Complete theme configuration
struct Theme {
    ThemeMode mode;
    
    // Current palette (updated based on mode)
    Color backgroundMain;
    Color panelBackground;
    Color accentColor;
    Color hoverOverlay;
    Color textPrimary;
    Color textSecondary;
    Color divider;
    Color shadowColor;

    // Design metrics
    static constexpr int CornerRadius = 12;              // 12px rounded corners
    static constexpr int ShadowBlur = 16;
    static constexpr int ShadowOffsetY = 4;
    
    // Spacing/padding
    static constexpr int PaddingLarge = 24;
    static constexpr int PaddingMedium = 16;
    static constexpr int PaddingSmall = 8;
    
    // Typography
    static constexpr int FontSizeHeading = 24;
    static constexpr int FontSizeBody = 14;
    static constexpr int FontSizeSmall = 12;
    
    // Animation durations (ms)
    static constexpr int AnimationFastMs = 100;
    static constexpr int AnimationNormalMs = 200;
    static constexpr int AnimationSlowMs = 300;

    Theme(ThemeMode m = ThemeMode::Light);
    
    void setMode(ThemeMode m) noexcept;
    [[nodiscard]] bool isDark() const noexcept { return mode == ThemeMode::Dark; }
    [[nodiscard]] bool isLight() const noexcept { return mode == ThemeMode::Light; }
};

// Global theme instance
extern Theme g_theme;

} // namespace notascore::ui
