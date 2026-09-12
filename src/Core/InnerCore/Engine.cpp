#include "Engine.hpp"

#include <OpenGL.hpp>

#include "../../Modules/Renderer/Renderer.hpp"
#include "../OuterCore/Service.hpp"
#include "Core/Services/ResourceManager.hpp"
#include "Modules/Input/Input.hpp"
#include "Modules/Physics/Physics.hpp"
#include "Modules/Profiling/Profiling.hpp"
#include "tracy/Tracy.hpp"
#include "tracy/TracyOpenGL.hpp"

namespace N
{
Engine::Engine()
{
    Window.Generate(800, 600, "Nova");
    if (Instance)
    {
        U::Logger::Fatal("Only one Engine may exist.");
    }
    Instance = this;
}

Engine& Engine::Get()
{
    return *Instance;
}

void Engine::Run()
{
    LastFrame = glfwGetTime();
    double accumulator = 0;
    Running = true;

    Start();
    while (Running)
    {
        BeginFrame();

        accumulator += DeltaTime;
        accumulator = std::min(accumulator, 1.0);

        while (accumulator >= FixedDeltaTime)
        {
            FixedUpdate();
            accumulator -= FixedDeltaTime;
        }

        Update();
        Render();
        EndFrame();
    }
    Stop();
}

void Engine::Shutdown()
{
    Running = false;
}

double Engine::GetTime() const
{
    return Time;
}

void Engine::Configure()
{
    Window.SetIcon({"Assets/icon.png"});
    // Window.SetSize(1980, 1200);
    glfwSwapInterval(0);

    Service::Add<ResourceManager>();
    Service::Add<EventBus>();

    AddModule<Input>();
    AddModule<Renderer>();
    AddModule<Profiling>();
    AddModule<Physics>();
}

void Engine::Start()
{
    TracyGpuContext;
    ZoneScopedN("Start");
    TracyGpuZone("Start");
    Configure();

    for (auto& service : Service::GetAll())
    {
        service->Start();
    }

    World.Start();

    for (auto& module : Modules)
    {
        module->Start();
    }
}

void Engine::Stop()
{
    ZoneScopedN("Stop");
    TracyGpuZone("Stop");
    auto& services = Service::GetAll();
    World.Stop();

    for (auto& module : Modules)
    {
        module->Stop();
    }

    for (auto& service : Service::GetAll())
    {
        service->Stop();
    }

    Service::DestroyServices();

    if (Instance == this)
    {
        Instance.Reset();
    }
}

void Engine::BeginFrame()
{
    ZoneScopedN("Begin Frame");
    TracyGpuZone("Begin Frame");

    const double currentTime = glfwGetTime();
    DeltaTime = currentTime - LastFrame;
    LastFrame = currentTime;
    Time = currentTime;

    Window.PollEvents();

    World.BeginFrame(DeltaTime);
    for (auto& module : Modules)
        module->BeginFrame(DeltaTime);

    for (auto& service : Service::GetAll())
    {
        service->BeginFrame(DeltaTime);
    }
}

void Engine::EndFrame()
{
    ZoneScopedN("End Frame");
    TracyGpuZone("End Frame");

    Window.SwapBuffers();
    World.EndFrame(DeltaTime);

    for (auto& module : Modules)
    {
        module->EndFrame(DeltaTime);
    }

    for (auto& service : Service::GetAll())
    {
        service->EndFrame();
    }

    FrameMark;
    TracyGpuCollect;
}

void Engine::Update()
{
    ZoneScopedN("Update");
    TracyGpuZone("Update");

    World.Update(DeltaTime);

    for (auto& module : Modules)
    {
        module->Update(DeltaTime);
    }

    for (auto& service : Service::GetAll())
    {
        service->Update(DeltaTime);
    }
}

void Engine::FixedUpdate()
{
    ZoneScopedN("Fixed Update");
    TracyGpuZone("Fixed Update");

    World.FixedUpdate(FixedDeltaTime);
    for (auto& module : Modules)
    {
        module->FixedUpdate(FixedDeltaTime);
    }

    for (auto& service : Service::GetAll())
    {
        service->FixedUpdate(FixedDeltaTime);
    }
}

void Engine::Render()
{
    ZoneScopedN("Render");
    TracyGpuZone("Render");

    World.Render();
    for (auto& module : Modules)
    {
        module->Render();
    }

    for (auto& service : Service::GetAll())
    {
        service->Render();
    }
}
} // namespace N
