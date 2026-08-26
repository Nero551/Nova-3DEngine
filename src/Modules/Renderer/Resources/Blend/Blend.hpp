#pragma once
#include "BlendEquation.hpp"
#include "BlendFactor.hpp"

namespace N {
struct Blend {
    bool Enabled;

    BlendFactor SourceRGB = BlendFactor::SourceAlpha;
    BlendFactor SourceAlpha = BlendFactor::One;

    BlendFactor DestinationRGB = BlendFactor::OneMinusSourceAlpha;
    BlendFactor DestinationAlpha = BlendFactor::OneMinusSourceAlpha;

    BlendEquation EquationRGB = BlendEquation::Add;
    BlendEquation EquationAlpha = BlendEquation::Add;

    M::Vector4 ConstantColor;

    Blend(bool enabled) : Enabled(enabled) {
    }

    void Apply() {
        if (!Enabled) {
            glDisable(GL_BLEND);
            return;
        }
        if (!glIsEnabled(GL_BLEND)) {
            glEnable(GL_BLEND);
        }

        glBlendEquationSeparate(static_cast<GLenum>(EquationRGB), static_cast<GLenum>(EquationAlpha));
        glBlendColor(ConstantColor.x, ConstantColor.y, ConstantColor.z, ConstantColor.w);

        glBlendFuncSeparate(static_cast<GLenum>(SourceRGB),
            static_cast<GLenum>(DestinationRGB),
            static_cast<GLenum>(SourceAlpha),
            static_cast<GLenum>(DestinationAlpha));
    }
};
} // namespace N
