#include "cubemaps.hpp"

#include "Core/OuterCore/Service.hpp"
#include "Core/Services/ResourceManager.hpp"
#include "Modules/Renderer/Resources/Texture/Texture.hpp"

namespace N {
void cubemaps::Start() {
    auto& resources = Service::Get<ResourceManager>();
    auto& cubemap = resources.Load<Texture>("Cubemap");
    cubemap.Target = TextureTarget::CubeMap;
}
} // namespace N
