#pragma once

namespace N {
/** @brief Base interface for all event types, providing polymorphic event identity. */
struct IEvent {
    virtual ~IEvent() = default;
};

/** @brief Self-managing event that owns its listeners and dispatches them immediately after Fire(). */
template <typename T> struct Event : IEvent {
    ~Event() override = default;

    /** @brief Dispatches the event to all registered listeners. */
    void Fire() {
        for (const auto& listener : Listeners) {
            listener.Callback(static_cast<T&>(*this));
        }
    }

    /** @brief Registers a listener and returns its unique subscription ID. */
    std::size_t Sub(const std::function<void(T&)>& listener) {
        const auto subscription = ++NextSubscription;
        Listeners.push_back({ .Subscription = subscription, .Callback = listener });
        return subscription;
    }

    /** @brief Removes a listener using its subscription ID. */
    void Unsub(std::size_t subscription) {
        int i = 0;
        while (i < Listeners.size() && Listeners.at(i).Subscription != subscription) {
            i++;
        }
        if (i < Listeners.size()) {
            Listeners.erase(Listeners.begin() + i);
        }
    }

private:
    /** @brief Stores a listener callback and its subscription ID. */
    struct Entry {
        std::size_t Subscription;
        std::function<void(T&)> Callback;
    };

    std::vector<Entry> Listeners;
    std::size_t NextSubscription = 0;
};
} // namespace N
