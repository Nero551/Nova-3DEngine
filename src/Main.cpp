#include <OpenGL.hpp>

#include "Core/InnerCore/Engine.hpp"
#include "Math/Common/Constants.hpp"
#include "Math/Common/Exponentials.hpp"
#include "Math/Common/Trigonometry.hpp"
#include "Math/Complex/Complex.hpp"
#include "Math/Functions/Function.hpp"
#include "Math/Quaternion.hpp"
#include "Math/Vector/Vector2.hpp"
#include "Math/Vector/Vector3.hpp"

static void Testing() {
    // E::M::Quaternion q(5, 1, 3, 2); // i
    // E::M::Quaternion p = E::M::Quaternion::Identity; // j
    //
    // E::U::Logger::Info(q.Magnitude());
    // E::U::Logger::Info(q.Normalized().Magnitude());
    //
    // E::M::Complex c = { 2, 5 };
    // E::U::Logger::Info(c * c.Inverse());

    // E::M::Complex c = E::M::Complex::FromPolar({ E::M::Rad(30), 3 });
    // E::M::Complex d = E::M::Complex::FromPolar({ E::M::Rad(45), 2 });
    // E::U::Logger::Info(E::M::Deg((c * d).Argument()));
    // E::U::Logger::Info((c * d).Magnitude());
    //
    // E::M::Vector2 a = E::M::Vector2::FromPolar({ E::M::Rad(30), 3 });
    // E::M::Vector2 b = E::M::Vector2::FromPolar({ E::M::Rad(45), 2 });
    // E::U::Logger::Info(E::M::Deg((a + b).Angle()));
    // E::U::Logger::Info((a + b).Length());
    //
    // E::M::Complex j = E::M::Complex::FromPolar({ E::M::Rad(30), 5 });
    // E::U::Logger::Info(j.Normalized().Magnitude());

    // E::M::Function cosine = [](const float x) {
    // return std::cos(x);
    // };
    // E::U::Logger::Info(cosine(0.6));
    // E::U::Logger::Info("[TAYLOR] ", cosine.Taylor(4, 0.5)(0.6));
}

int main() {
    // Testing();
    // return 0;
    E::Engine engine;
    engine.Run();
    return 0;
}
