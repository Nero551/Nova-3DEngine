#pragma once
#include "Enums/StencilAction.hpp"
#include "Enums/StencilFunction.hpp"

namespace N {
struct Stencil {
    bool Enabled;

    int Ref = 1;
    unsigned int Mask = 0xFF;
    StencilFunction Function = StencilFunction::Always;

    /** @brief Action to take if stencil test fails. */
    StencilAction SFail = StencilAction::Keep;

    /** @brief Action to take if stencil test passes but depth test fails . */
    StencilAction DFail = StencilAction::Keep;

    /** @brief Action to take if stencil & depth tests pass. */
    StencilAction SDPass = StencilAction::Keep;


    Stencil(bool enabled) : Enabled(enabled) {
    }

    void Apply() {
        if (!Enabled) {
            return;
        }
        glStencilOp(static_cast<GLenum>(SFail), static_cast<GLenum>(DFail), static_cast<GLenum>(SDPass));
        glStencilMask(Mask);
        glStencilFunc(static_cast<GLenum>(Function), Ref, Mask);
    }
};
} // namespace N
