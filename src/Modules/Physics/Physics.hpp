#pragma once

#include "Core/InnerCore/Module.hpp"

namespace N {
struct Physics : Module {
protected:
    void Start() override;
    void FixedUpdate(double fdt) override;
    void AddSystems() override;
};
} // namespace N
