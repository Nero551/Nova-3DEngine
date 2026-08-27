#include "../Resources/Texture/Texture.hpp"
#include "Core/Services/ResourceManager.hpp"
#include "Modules/Renderer/Resources/Texture/Texture2D.hpp"
#include "Primitives.hpp"

namespace N {
Texture& Primitives::CreateWhiteTexture() {
    std::vector<unsigned char> white = { 255, 255, 255, 255 };
    U::Image image = { 1, 1, U::Image::ColorChannels::RGBA, white };
    auto& whiteTexture = Service::Get<ResourceManager>().Load<Texture2D>("WhiteTexture");
    whiteTexture.UseImage(image);

    return whiteTexture;
}

Texture& Primitives::CreateBlackTexture() {
    std::vector<unsigned char> black = { 0, 0, 0, 255 };
    U::Image image = { 1, 1, U::Image::ColorChannels::RGBA, black };
    auto& blackTexture = Service::Get<ResourceManager>().Load<Texture2D>("BlackTexture");
    blackTexture.UseImage(image);
    return blackTexture;
}
} // namespace N
