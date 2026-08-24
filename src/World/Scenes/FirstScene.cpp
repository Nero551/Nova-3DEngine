#include "FirstScene.hpp"

#include "../../Modules/Renderer/Primitives/Primitives.hpp"
#include "AssimpScene.hpp"
#include "Core/Services/ResourceManager.hpp"
#include "Modules/Renderer/Uniforms/Vector3Uniform.hpp"
#include "World/Novas/Light.hpp"
#include "World/Novas/MeshInstance3D.hpp"

namespace E {
FirstScene::FirstScene() {
    SetRoot(World::Get().CreateEntity<Nova3D>());
}
} // namespace E
