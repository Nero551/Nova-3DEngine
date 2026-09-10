#include "Core/InnerCore/Engine.hpp"
#include "Math/Matrix/Matrix4.hpp"
#include "Math/Quaternion/Quaternion.hpp"
#include "Utilities/Reflection.hpp"

static void Testing() {}
int main()
{

    N::U::Logger::Info(*REGISTER_ATTRIBUTE(N::M::Quaternion, x).GetType());
    N::U::Logger::Info(REGISTER_ATTRIBUTE(N::M::Quaternion, w));
    Testing();
    // return 0;
    N::Engine engine;
    engine.Run();
    return 0;
}
