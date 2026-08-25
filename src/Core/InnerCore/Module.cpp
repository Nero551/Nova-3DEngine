#include "Module.hpp"

namespace N {
void Module::Start() {
    AddSystems();
    OnStart();
    for (auto& system : Systems | std::views::values) {
        system->Start();
    }
}

void Module::Update(const double dt) {
    OnUpdate(dt);
    for (auto& system : Systems | std::views::values) {
        system->Update(dt);
    }
}

void Module::FixedUpdate(const double fdt) {
    OnFixedUpdate(fdt);
    for (auto& system : Systems | std::views::values) {
        system->FixedUpdate(fdt);
    }
}

void Module::Stop() {
    OnStop();
    for (auto& system : Systems | std::views::values) {
        system->Stop();
    }
}

void Module::Render() {
    OnRender();
    for (auto& system : Systems | std::views::values) {
        system->Render();
    }
}

void Module::BeginFrame(const double dt) {
    OnBeginFrame(dt);
    for (auto& system : Systems | std::views::values) {
        system->BeginFrame(dt);
    }
}

void Module::EndFrame(const double dt) {
    OnEndFrame(dt);
    for (auto& system : Systems | std::views::values) {
        system->EndFrame(dt);
    }
}
} // namespace N
