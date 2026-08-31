#include "World.hpp"

#include "Engine.hpp"
#include "Modules/Input/Enums/Keys.hpp"
#include "Modules/Input/Input.hpp"
#include "World/Events/EntityDestroyed.hpp"
#include "World/Novas/Camera.hpp"
#include "World/Scenes/CoordinateAxesScene.hpp"
#include "World/Scenes/FirstScene.hpp"
#include "World/Systems/Transform3DSystem.hpp"
#include "World/experiments/calculus.hpp"

namespace N {
World& World::Get() {
    return Engine::Get().World;
}

void World::RemoveEntity(const unsigned int id) {
    auto entity = TryFindEntity(id);
    if (!entity) {
        return;
    }
    const auto descendants = entity->GetDescendants();

    if (entity->HasParent()) {
        entity->ClearParent();
    }

    if (entity->Id == Root->Id) {
        Root.Reset();
    }

    Service::Get<EventBus>().Fire<EntityDestroyed>(*entity);
    Entities.erase(id);

    for (auto& descendant : descendants) {
        Service::Get<EventBus>().Fire<EntityDestroyed>(*descendant);
        Entities.erase(descendant->Id);
    }
}

Entity& World::FindEntity(unsigned int id) {
    auto entity = Entities.find(id);
    if (entity == Entities.end()) {
        U::Logger::Fatal(std::format("Entity Not Found: {}", id));
    }
    return *entity->second;
}

U::CheckedPtr<Entity> World::TryFindEntity(const unsigned int id) {
    auto entity = Entities.find(id);

    if (entity == Entities.end()) {
        return nullptr;
    }

    return entity->second.get();
}

void World::AddSystems() {
    AddSystem<Transform3DSystem>();
    AddSystem<calculus>();
}


// TODO- quick flicker happens at the start of the run, its input mouse rapidly changing when changing MouseMode.
void World::Start() {
    AddSystems();
    Engine::Get().GetModule<Input>().SetMouseMode(MouseMode::Disabled);

    Root = &CreateEntity<Nova>();

    auto& camera = CreateEntity<Camera>();
    Query.Pool<Transform3DComponent>().GetComponentById(camera.Id).Position = { 0, 0, 10 };
    Root->AttachChild(camera);
    ActiveCamera = &camera;

    CoordinateAxesScene coordinateAxes;
    Root->AttachChild(coordinateAxes.GetRoot());

    FirstScene firstScene;
    coordinateAxes.GetRoot().AttachChild(firstScene.GetRoot());

    for (auto& system : Systems | std::views::values) {
        system->Start();
    }
}

void World::Update(const double dt) {
    if (Engine::Get().GetModule<Input>().IsKeyHeld(Key::Escape)) {
        Engine::Get().Shutdown();
    }

    if (Engine::Get().GetModule<Input>().IsKeyReleased(Key::Q)) {
        if (Engine::Get().GetModule<Input>().GetMouseMode() == MouseMode::Disabled) {
            Engine::Get().GetModule<Input>().SetMouseMode(MouseMode::Normal);
        }
        else {
            Engine::Get().GetModule<Input>().SetMouseMode(MouseMode::Disabled);
        }
    }

    for (auto& system : Systems | std::views::values) {
        system->Update(dt);
    }
}

void World::FixedUpdate(const double fdt) {
    for (auto& system : Systems | std::views::values) {
        system->FixedUpdate(fdt);
    }
}

void World::Stop() {
    for (auto& system : Systems | std::views::values) {
        system->Stop();
    }
}

void World::BeginFrame(const double dt) {
    for (auto& system : Systems | std::views::values) {
        system->BeginFrame(dt);
    }
}

void World::EndFrame(const double dt) {
    for (auto& system : Systems | std::views::values) {
        system->EndFrame(dt);
    }
}

void World::Render() {
    for (auto& system : Systems | std::views::values) {
        system->Render();
    }
}
} // namespace N
