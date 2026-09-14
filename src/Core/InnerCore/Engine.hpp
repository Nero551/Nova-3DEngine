#pragma once
#include "GraphicsContext.hpp"
#include "Module.hpp"
#include "Utilities/CheckedPtr.hpp"
#include "Window.hpp"
#include "World.hpp"

/** @brief Root namespace for Nova engine functionality. */
namespace N
{
template <typename T>
concept ModuleType = std::derived_from<T, Module>;

/**
 * @brief Main Nova engine and game loop.
 * Manages the window, world, modules, and frame lifecycle.
 */
struct Engine
{
    Window Window;
    std::optional<World> World;

    Engine(const Engine&) = delete;
    Engine& operator=(const Engine&) = delete;
    Engine(Engine&&) = delete;
    Engine& operator=(Engine&&) = delete;

    /** @brief Constructs the engine and registers it as the global instance. */
    Engine();

    /** @brief Returns the global engine instance. */
    static Engine& Get();

    /**
     * @brief Runs the engine's main loop.
     * @remark Order: Start -> BeginFrame -> FixedUpdate -> Update -> Render -> EndFrame
     * -> Stop
     */
    void Run();

    /** @brief Requests the engine to shut down. */
    void Shutdown();

    /** @brief Returns elapsed engine time in seconds. */
    double GetTime() const;

    /** @brief Returns a registered module by type. */
    template <ModuleType T> T& GetModule()
    {
        auto module = Modules.Find<T>();
        if (module == Modules.end())
        {
            U::Log::Fatal(std::format("Module {} not found", typeid(T).name()));
        }
        return static_cast<T&>(**module);
    }

    const TypedVector<std::unique_ptr<Module>>& GetAllModules() const
    {
        return Modules;
    }

  private:
    GraphicsContext GraphicsContext{};

    /** @brief Global engine instance. */
    inline static U::CheckedPtr<Engine> Instance = nullptr;

    /** @brief Registered engine modules. */
    TypedVector<std::unique_ptr<Module>> Modules;

    double LastFrame = 0;
    bool Running = false;
    double Time = 0;
    double DeltaTime = 0;
    double FixedDeltaTime = 1.0 / 60.0;

    /** @brief Initializes engine services, modules & custom configurations. */
    void Configure();

    /** @brief Creates and registers a module. */
    template <ModuleType T> T& AddModule()
    {
        auto module = std::make_unique<T>();
        Modules.Emplace<T>(std::move(module));
        return static_cast<T&>(*Modules.GetUnchecked<T>());
    }

    void Start();
    void BeginFrame();
    void FixedUpdate();
    void Update();
    void Render();
    void EndFrame();
    void Stop();
};
} // namespace N
