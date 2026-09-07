#include "FirstScene.hpp"

#include "AssimpScene.hpp"
#include "Modules/Renderer/Resources/Shader/Uniforms/Vector3Uniform.hpp"
#include "World/Novas/MeshInstance3D.hpp"

namespace N {
FirstScene::FirstScene()
{
    SetRoot(World::Get().CreateEntity<Nova3D>());
}
} // namespace N
