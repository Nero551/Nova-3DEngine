#pragma once
#include "Core/OuterCore/Entity.hpp"
namespace N {
struct Entity : Entity {
   void Initialize() override {
    Entity::Initialize();
   }
};
}