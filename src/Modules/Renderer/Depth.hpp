#pragma once
#include "Enums/DepthFunction.hpp"

namespace N {
struct Depth {
    bool Enabled = true;
    bool Write = true;
    DepthFunction Function = DepthFunction::Less;

    Depth(bool enabled) : Enabled(enabled) {
    }

    void Apply() {
        if (!Enabled) {
            glDisable(GL_DEPTH_TEST);
            return;
        }
        if (!glIsEnabled(GL_DEPTH_TEST)) {
            glEnable(GL_DEPTH_TEST);
        }
        glDepthMask(Write);
        glDepthFunc(static_cast<GLenum>(Function));
    }
};
} // namespace N
