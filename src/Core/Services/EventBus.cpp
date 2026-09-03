#include "EventBus.hpp"

namespace N {
void EventBus::EmptyFireQueue() {
    for (auto& event : FireQueue) {
        auto listeners = Listeners.find(typeid(*event));
        if (listeners == Listeners.end()) {
            continue;
        }

        for (auto& listener : listeners->second) {
            listener.Callback(*event);
        }
    }
    FireQueue.clear();
}

void EventBus::EndFrame() {
    EmptyFireQueue();
}
} // namespace N
