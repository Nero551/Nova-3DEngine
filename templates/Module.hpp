#pragma once

#include "Core/InnerCore/Module.hpp"
namespace N {
struct Module : C::Module {
protected:
   void OnStart() override;
   void OnUpdate(double dt) override;
};
}