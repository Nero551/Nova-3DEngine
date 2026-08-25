#pragma once
#include "GraphicsContext.hpp"
#include "Module.hpp"
#include "Utilities/CheckedPtr.hpp"
#include "Window.hpp"
#include "World.hpp"

/** @brief Root namespace for Nova engine functionality. */
namespace N {
template <typename T>
concept ModuleType = std::derived_from<T, Module>;

/**
 * @brief Main Nova engine and game loop.
 * Manages the window, world, modules, and frame lifecycle.
 */
struct Engine {
private:
    GraphicsContext GraphicsContext{};

public:
    Window Window;
    World World;

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
     * @remark Order: Start -> BeginFrame -> FixedUpdate -> Update -> Render -> EndFrame -> Stop
     */
    void Run();

    /** @brief Requests the engine to shut down. */
    void Shutdown();

    /** @brief Returns elapsed engine time in seconds. */
    double GetTime() const;

    /** @brief Returns a registered module by type. */
    template <ModuleType T> T& GetModule() {
        auto module = Modules.find(typeid(T));
        if (module == Modules.end()) {
            U::Logger::Fatal(std::format("Module {} not found", typeid(T).name()));
        }
        return static_cast<T&>(*module->second);
    }

private:
    /** @brief Global engine instance. */
    inline static U::CheckedPtr<Engine> Instance = nullptr;

    /** @brief Registered engine modules. */
    std::unordered_map<std::type_index, std::unique_ptr<Module>> Modules;

    /** @brief Previous frame start time. in seconds */
    double LastFrame = 0;

    bool Running = false;

    /** @brief Total elapsed engine time in seconds. */
    double Time = 0;

    /** @brief Previous frame duration in seconds. */
    double DeltaTime = 0;

    /** @brief Fixed update timestep in seconds. */
    double FixedDeltaTime = 1.0 / 60.0;

    /** @brief Initializes engine services, modules & custom configurations. */
    void Configure();

    /** @brief Creates and registers a module. */
    template <ModuleType T> T& AddModule() {
        auto module = std::make_unique<T>();
        Modules.emplace(typeid(T), std::move(module));
        return static_cast<T&>(*Modules.find(typeid(T))->second);
    }

    /** @brief Starts the engine and its systems. */
    void Start();

    /** @brief Stops the engine and cleans up its systems. */
    void Stop();

    /** @brief Begins a frame. */
    void BeginFrame();

    /** @brief Ends a frame. */
    void EndFrame();

    /** @brief Runs update step. */
    void Update();

    /** @brief Runs the fixed update step. */
    void FixedUpdate();

    /** @brief Runs render step. */
    void Render();
};
} // namespace N
