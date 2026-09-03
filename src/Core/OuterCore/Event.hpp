#pragma once

namespace N {
/**
 * @brief Base event type that manages a collection of listeners.
 *
 * Listeners are registered through Sub() and removed through their
 * subscription ID returned by Sub().
 */

struct IEvent {
    virtual ~IEvent() = default;
};

template <typename T> struct Event : IEvent {
    ~Event() override = default;

    void Fire() {
        for (const auto& listener : Listeners) {
            listener.Callback(static_cast<T&>(*this));
        }
    }

    std::size_t Sub(const std::function<void(T&)>& listener) {
        const auto subscription = ++NextSubscription;
        Listeners.push_back({ .Subscription = subscription, .Callback = listener });
        return subscription;
    }

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
    struct Entry {
        std::size_t Subscription;
        std::function<void(T&)> Callback;
    };

    std::vector<Entry> Listeners;
    std::size_t NextSubscription = 0;
};
} // namespace N
