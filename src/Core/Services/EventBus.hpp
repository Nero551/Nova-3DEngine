#pragma once

#include "../OuterCore/Event.hpp"
#include "Core/OuterCore/Service.hpp"

namespace N {
template <typename T>
concept EventType = std::derived_from<T, Event>;

// TODO- redesign this whole event system.
//  preferably, Events would be able to handle themselves, meaning ability to sub, unsub, fire all on their own.
//  EventBus will just be a organized queue that can be used and can't, depending on the situation.

struct EventBus : Service {
    template <EventType T, typename... Args> void InstantFire(Args&&... args) {
        auto listeners = Listeners.find(typeid(T));
        if (listeners == Listeners.end()) {
            return;
        }

        T event{ std::forward<Args>(args)... };
        for (auto& callback : listeners->second) {
            callback(event);
        }
    }

    template <EventType T, typename... Args> void Fire(Args&&... args) {
        if constexpr (!std::constructible_from<T, Args...>) {
            U::Logger::Fatal(std::string("Event: ") + typeid(T).name() + " Can't Be Constructed From the Given Arguments.");
        }
        else {
            auto event = std::make_unique<T>(std::forward<Args>(args)...);
            FireQueue.emplace_back(std::move(event));
        }
    }

    void EmptyFireQueue();

    template <EventType T, typename F> void Sub(F&& callback) {
        auto method = [callback](Event& e) { callback(static_cast<T&>(e)); };

        Listeners[typeid(T)].emplace_back(method);
    }

    template <EventType T> void UnSub(const std::function<void(T&)>& callback) {
    }

private:
    std::unordered_map<std::type_index, std::vector<std::function<void(Event&)>>> Listeners;
    std::vector<std::unique_ptr<Event>> FireQueue;

protected:
    void EndFrame() override;
};
} // namespace N
