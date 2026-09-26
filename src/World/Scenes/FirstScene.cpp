#include "FirstScene.hpp"

#include "AssimpScene.hpp"
#include "Modules/Graphics/Novas/MeshInstance3D.hpp"
#include "Modules/Graphics/Resources/Shader/Uniforms/Vector3Uniform.hpp"

namespace N
{
FirstScene::FirstScene()
{
    SetRoot(C::World::Get().CreateEntity<Nova3D>());
}
} // namespace N
