#pragma once
#include "Enums/StencilAction.hpp"
#include "Enums/StencilFunction.hpp"

namespace N {
struct Stencil {
    bool Enabled = false;

    int Ref = 1;

    /** @brief Bit mask applied to stencil values when comparing against Ref. */
    unsigned int FunctionMask = 0xFF;

    /** @brief Comparison function used by the stencil test. */
    StencilFunction Function = StencilFunction::Always;

    /** @brief Bit mask controlling which stencil bits can be written. */
    unsigned int WriteMask = 0xFF;

    /** @brief Action to take if the stencil test fails. */
    StencilAction SFail = StencilAction::Keep;

    /** @brief Action to take if the stencil test passes but the depth test fails. */
    StencilAction DFail = StencilAction::Keep;

    /** @brief Action to take if both stencil and depth tests pass. */
    StencilAction SDPass = StencilAction::Keep;

    Stencil(const bool enabled) : Enabled(enabled) {
    }

    void Apply() {
        if (!Enabled) {
            glDisable(GL_STENCIL_TEST);
            return;
        }

        if (!glIsEnabled(GL_STENCIL_TEST)) {
            glEnable(GL_STENCIL_TEST);
        }

        glStencilOp(static_cast<GLenum>(SFail), static_cast<GLenum>(DFail), static_cast<GLenum>(SDPass));
        glStencilMask(WriteMask);
        glStencilFunc(static_cast<GLenum>(Function), Ref, FunctionMask);
    }
};
} // namespace N
