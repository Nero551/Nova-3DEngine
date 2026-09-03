#pragma once

#include "../OuterCore/Event.hpp"
#include "Core/OuterCore/Service.hpp"

namespace N {
template <typename T>
concept EventType = std::derived_from<T, IEvent>;

// TODO- redesign this whole event system.
//  preferably, Events would be able to handle themselves, meaning ability to sub, unsub, fire all on their own.
//  EventBus will just be a organized queue that can be used and can't, depending on the situation.

struct EventBus : Service {
    template <EventType T, typename... Args> void InstantFire(Args&&... args) {
        if constexpr (!std::constructible_from<T, Args...>) {
            U::Logger::Fatal(std::string("Event: ") + typeid(T).name() + " Can't Be Constructed From the Given Arguments.");
        }
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
        auto event = std::make_unique<T>(std::forward<Args>(args)...);
        FireQueue.emplace_back(std::move(event));
    }

    void EmptyFireQueue();

    template <EventType T, typename F> std::size_t Sub(F&& callback) {
        auto method = [callback = std::forward<F>(callback)](IEvent& e) mutable { callback(static_cast<T&>(e)); };

        const auto subscription = ++NextSubscription;
        Listeners[typeid(T)].push_back({ .Subscription = subscription, .Callback = method });
        return subscription;
    }

    template <EventType T> void UnSub(std::size_t subscription) {
        auto it = Listeners.find(typeid(T));
        if (it != Listeners.end()) {
            size_t i = 0;
            while (i < it->second.size() && it->second.at(i).Subscription != subscription) {
                i++;
            }
            if (i < it->second.size()) {
                it->second.erase(it->second.begin() + i);
                if (it->second.empty()) {
                    Listeners.erase(it);
                }
            }
        }
    }

private:
    struct Entry {
        std::size_t Subscription;
        std::function<void(IEvent&)> Callback;
    };

    std::unordered_map<std::type_index, std::vector<Entry>> Listeners;
    std::vector<std::unique_ptr<IEvent>> FireQueue;
    size_t NextSubscription = 0;

protected:
    void EndFrame() override;
};
} // namespace N
