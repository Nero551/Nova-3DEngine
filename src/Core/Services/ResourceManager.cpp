#include "ResourceManager.hpp"

namespace N {
void ResourceManager::Unload(const std::string& name) {
    if (Resources.contains(name)) {
        Resources.erase(name);
    }
}
} // namespace N
