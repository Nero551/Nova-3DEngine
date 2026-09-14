#include "Core/InnerCore/Engine.hpp"
static void Testing() {}
int main()
{
    //
    // N::U::Logger::Info(REGISTER_ATTRIBUTE(N::M::Quaternion, x));
    // N::U::Logger::Info(REGISTER_ATTRIBUTE(N::M::Quaternion, y));
    // N::U::Logger::Info(REGISTER_ATTRIBUTE(N::M::Quaternion, z));
    // N::U::Logger::Info(REGISTER_ATTRIBUTE(N::M::Quaternion, w));
    Testing();
    // return 0;
    N::Engine engine;
    engine.Run();
    return 0;
}
