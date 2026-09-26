#pragma once
#include "Core/OuterCore/Entity.hpp"
namespace N {
struct Entity : C::Entity {
   void Initialize() override {
    Entity::Initialize();
   }
};
}