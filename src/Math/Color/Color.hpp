#pragma once
#include "../Vector/Vector4.hpp"

namespace N::M {
struct Color {
    inline static const Vector4 White = { 1.0f, 1.0f, 1.0f, 1.0f };
    inline static const Vector4 Black = { 0.0f, 0.0f, 0.0f, 1.0f };

    inline static const Vector4 Red = { 1.0f, 0.0f, 0.0f, 1.0f };
    inline static const Vector4 Green = { 0.0f, 1.0f, 0.0f, 1.0f };
    inline static const Vector4 Blue = { 0.0f, 0.0f, 1.0f, 1.0f };

    inline static const Vector4 Yellow = { 1.0f, 1.0f, 0.0f, 1.0f };
    inline static const Vector4 Cyan = { 0.0f, 1.0f, 1.0f, 1.0f };
    inline static const Vector4 Magenta = { 1.0f, 0.0f, 1.0f, 1.0f };

    inline static const Vector4 Gray = { 0.5f, 0.5f, 0.5f, 1.0f };
    inline static const Vector4 LightGray = { 0.75f, 0.75f, 0.75f, 1.0f };
    inline static const Vector4 DarkGray = { 0.25f, 0.25f, 0.25f, 1.0f };

    inline static const Vector4 Orange = { 1.0f, 0.5f, 0.0f, 1.0f };
    inline static const Vector4 Purple = { 0.5f, 0.0f, 1.0f, 1.0f };
    inline static const Vector4 Brown = { 0.6f, 0.3f, 0.1f, 1.0f };

    inline static const Vector4 Pink = { 1.0f, 0.4f, 0.7f, 1.0f };

    inline static const Vector4 Transparent = { 0.0f, 0.0f, 0.0f, 0.0f };
};
} // namespace N::M
