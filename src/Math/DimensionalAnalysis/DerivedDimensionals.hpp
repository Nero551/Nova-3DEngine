#pragma once
#include "FundamentalDimensionals.hpp"
#include "OperationalDimensionals.hpp"
namespace N::M
{
using Velocity = DivideDimensional<Length<1>, Time<1>>;
using Acceleration = DivideDimensional<Length<1>, Time<2>>;

} // namespace N::M