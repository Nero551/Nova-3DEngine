#pragma once

#include "Core/OuterCore/ECS/Component.hpp"
#include "Core/OuterCore/Track.hpp"
#include "Math/Matrix/Matrix4.hpp"
#include "Math/Quaternion/Quaternion.hpp"

namespace N
{
//
struct Transform3DComponent : Component
{
    Track<M::Vector3> Position = M::Vector3::Zero;
    Track<M::Quaternion> Rotation = M::Quaternion::Identity;
    Track<M::Vector3> Scale = M::Vector3::One;

    bool InheritTransform = true;

  private:
    M::Matrix4 DirtyModelMatrix;
    M::Matrix3 DirtyNormalMatrix;

  public:
    [[nodiscard]] M::Matrix4 GetModelMatrix()
    {
        if (GlobalPosition.IsChanged() || GlobalRotation.IsChanged() || GlobalScale.IsChanged())
        {
            M::Matrix4 modelMatrix = M::Matrix4::Identity;
            modelMatrix = modelMatrix.Translate({GlobalPosition});
            modelMatrix *= GlobalRotation().ToMatrix4();
            modelMatrix = modelMatrix.Scale(GlobalScale);
            DirtyModelMatrix = modelMatrix;
            Position.ClearChanged();
            Rotation.ClearChanged();
            Scale.ClearChanged();
        }

        return DirtyModelMatrix;
    }

    [[nodiscard]] M::Matrix3 GetNormalMatrix()
    {
        if (GlobalPosition.IsChanged() || GlobalRotation.IsChanged() || GlobalScale.IsChanged())
        {
            DirtyNormalMatrix = GetModelMatrix().ToMatrix3().Inverse().Transpose();
            return DirtyNormalMatrix;
        }
        return DirtyNormalMatrix;
    }

    [[nodiscard]] M::Vector3 GetRight() const
    {
        return GlobalRotation().Transform({1, 0, 0});
    }

    [[nodiscard]] M::Vector3 GetUp() const
    {
        return GlobalRotation().Transform({0, 1, 0});
    }

    [[nodiscard]] M::Vector3 GetForward() const
    {
        return GlobalRotation().Transform({0, 0, -1});
    }

  private:
    // TODO-this is temporary until i have a proper change detection system (reflection)
    Track<M::Vector3> GlobalPosition = M::Vector3::Zero;
    Track<M::Quaternion> GlobalRotation = M::Quaternion::Identity;
    Track<M::Vector3> GlobalScale = M::Vector3::One;
    friend struct Transform3DSystem;
    friend struct Renderer;
    friend struct CameraSystem;
};
} // namespace N
