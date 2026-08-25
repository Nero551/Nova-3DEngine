#pragma once
#include "Utilities/CheckedPtr.hpp"
#include "Utilities/Logger.hpp"

namespace N {
struct Service;
template <typename T>
concept ServiceType = std::derived_from<T, Service>;

/**
 * @brief Base class for globally accessible services.
 * Services aren't owned by the Engine, primarily to avoid cyclic dependencies.
 * services do get destroyed ,created & updated in the engine loop though.
 */
struct Service {
    Service() = default;
    virtual ~Service() = default;
    Service(const Service&) = delete;
    Service& operator=(const Service&) = delete;

    /**
     * @brief Retrieves a registered service by type.
     * @tparam T Type of the service to retrieve.
     * @return Reference to the registered service.
     */
    template <ServiceType T> static T& Get() {
        auto service = Services.find(typeid(T));
        if (service == Services.end()) {
            U::Logger::Fatal(std::format("Service Not Found: {}", typeid(T).name()));
        }
        return static_cast<T&>(*service->second);
    }

    /**
     * @brief Returns all currently registered services.
     * @return A vector of pointers to the registered services.
     */
    static std::vector<U::CheckedPtr<Service>> GetAll() {
        std::vector<U::CheckedPtr<Service>> services;

        for (auto& service : Services | std::views::values) {
            services.emplace_back(&*service);
        }
        return services;
    }

protected:
    friend struct Engine;
    virtual void Start() {
    }
    virtual void Update(double dt) {
    }
    virtual void FixedUpdate(double fdt) {
    }
    virtual void Render() {
    }
    virtual void BeginFrame(double dt) {
    }
    virtual void EndFrame() {
    }
    virtual void Stop() {
    }

    /**
     * @brief Registers a new service of the specified type.
     * If a service of the same type is already registered, the existing
     * service is returned instead.
     * @tparam T Type of the service to register.
     * @return Reference to the registered service.
     */
    template <ServiceType T> static T& Add() {
        if (Services.contains(typeid(T))) {
            U::Logger::Error(std::format(" Service {} Already Added", typeid(T).name()));
            return static_cast<T&>(*Services.at(typeid(T)));
        }

        auto service = std::make_unique<T>();
        Services.emplace(std::type_index(typeid(T)), std::move(service));

        return Get<T>();
    }

private:
    /** @brief Destroys all services */
    static void DestroyServices() {
        Services.clear();
    }

    inline static std::unordered_map<std::type_index, std::unique_ptr<Service>> Services;
};
} // namespace N
