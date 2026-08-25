#pragma once

#include "Core/InnerCore/Module.hpp"

namespace N {
struct Physics : Module {
protected:
    void OnStart() override;
    void OnFixedUpdate(double fdt) override;
    void AddSystems() override;
};
} // namespace N
