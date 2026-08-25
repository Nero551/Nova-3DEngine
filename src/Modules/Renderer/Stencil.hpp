#pragma once
namespace N {
enum class StencilFunction {
    Never = GL_NEVER,
    Less = GL_LESS,
    Equal = GL_EQUAL,
    LessEqual = GL_LEQUAL,
    Greater = GL_GREATER,
    NotEqual = GL_NOTEQUAL,
    GreaterEqual = GL_GEQUAL,
    Always = GL_ALWAYS
};

enum class StencilAction : GLenum {
    Keep = GL_KEEP,
    Zero = GL_ZERO,
    Replace = GL_REPLACE,
    Increment = GL_INCR,
    IncrementWrap = GL_INCR_WRAP,
    Decrement = GL_DECR,
    DecrementWrap = GL_DECR_WRAP,
    Invert = GL_INVERT
};

struct Stencil {
    bool Enabled;

    int Ref = 1;
    unsigned int Mask = 0xFF;
    StencilFunction Function = StencilFunction::Less;

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
        glStencilFunc(static_cast<GLenum>(Function), Ref, Mask);
        glStencilOp(static_cast<GLenum>(SFail), static_cast<GLenum>(DFail), static_cast<GLenum>(SDPass));
    }
};
} // namespace N
