#pragma once
#include "pch.h"
#include <cocos2d.h>
#include <imgui.h>
#include <nlohmann/json.hpp>
#include <stdint.h>

struct Vec2
{
    float x, y;
};

struct Color
{
    float r, g, b, a;

    inline ImVec4 to_imvec4() const
    {
        return ImVec4(r, g, b, a);
    }

    inline uint32_t to_uint32() const
    {
        return (uint32_t)(r * 255) |
               (uint32_t)(g * 255) << 8 |
               (uint32_t)(b * 255) << 16 |
               (uint32_t)(a * 255) << 24;
    }

    inline std::string to_hex() const
    {
        uint32_t hex_int = to_uint32();
        return fmt::format("{:08X}", hex_int);
    }

    static inline Color from_hex(std::string const &hex)
    {
        if (hex.size() != 8)
            throw std::runtime_error("Invalid hex string");

        uint32_t hex_int = std::stoul(hex, nullptr, 16);
        return Color{
            (float)((hex_int >> 0) & 0xFF) / 255.0f,
            (float)((hex_int >> 8) & 0xFF) / 255.0f,
            (float)((hex_int >> 16) & 0xFF) / 255.0f,
            (float)((hex_int >> 24) & 0xFF) / 255.0f};
    }
};

class EaseInAnimation;

namespace globals
{
    using json = nlohmann::json;

    extern Vec2 screen_size; // screen size ratio

    extern bool reset_windows;    // flag to reset window positions
    extern json window_positions; // stored window positions

    extern float animation;                   // window animation timer
    extern bool animation_done;               // flag to check if animation is finished
    extern int random_direction;              // random direction for window animation
    extern EaseInAnimation *animation_action; // animation action

    extern bool is_december; // flag to check if it's december (for snow effect in main menu)

    extern uint32_t blur_shader; // handle to compiled blur shader program

    extern ImFont *main_font;  // font for all elements
    extern ImFont *title_font; // font for window titles

    extern ImVec4 current_color; // current color for ImGui elements (either rainbow or config::text_color)

    extern char search[64]; // buffer for search bar
}