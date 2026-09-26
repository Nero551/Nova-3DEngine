#pragma once

#include "Math/Common/Comparison.hpp"
#include "Math/Common/Constants.hpp"
#include "Math/Common/Interpolation.hpp"
#include "Math/Coordinates/Polar.hpp"
#include "Utilities/Log.hpp"
#include "Vector.hpp"

namespace N::M
{
/**
 * @brief 2D floating-point vector.
 *
 * Components:
 * - x: horizontal component.
 * - y: vertical component.
 *
 * Vectors are treated as column vectors when used with matrices.
 */
template <> struct Vector<2>
{
    float x = 0;
    float y = 0;

    /** @brief Returns a component by index. */
    constexpr float& operator()(const unsigned int component)
    {
        switch (component)
        {
        case 0:
            return x;
        case 1:
            return y;
        default:
            U::Log::Fatal("Index : ", component, " doesn't exist in vector(x, y)");
        }
    }

    /** @brief Returns a component by index. */
    constexpr const float& operator()(const unsigned int component) const
    {
        switch (component)
        {
        case 0:
            return x;
        case 1:
            return y;
        default:
            U::Log::Fatal("Index : ", component, " doesn't exist in vector(x, y)");
        }
    }

    /** @brief Returns the zero vector. */
    static constexpr Vector Zero()
    {
        return Vector{0};
    }

    /** @brief Constructs a zero vector. */
    constexpr Vector() = default;

    /** @brief Constructs a vector with all components set to the same value. */
    explicit constexpr Vector(const float all) : x(all), y(all) {}

    /** @brief Constructs a vector from individual components. */
    constexpr Vector(const float x, const float y) : x(x), y(y) {}

    /** @brief Constructs a vector from polar coordinates. */
    static constexpr Vector FromPolar(const Polar& polar)
    {
        return {polar.Magnitude * std::cos(polar.Angle), polar.Magnitude * std::sin(polar.Angle)};
    }

    /** @brief Returns the squared length of the vector. */
    constexpr float LengthSquared() const
    {
        return x * x + y * y;
    }

    /** @brief Returns the length of the vector. */
    constexpr float Length() const
    {
        return std::sqrt(LengthSquared());
    }

    /** @brief Returns the angle of the vector from the positive x-axis. */
    constexpr float Angle() const
    {
        return std::atan2(y, x);
    }

    /** @brief Returns a normalized copy of the vector. */
    constexpr Vector Normalized() const
    {
        const float length = Length();

        if (length == 0)
        {
            return Zero();
        }

        return {x / length, y / length};
    }

    /** @brief Returns the dot product with another vector. */
    constexpr float Dot(const Vector& vector) const
    {
        return x * vector.x + y * vector.y;
    }

    /** @brief Linearly interpolates between this vector and another vector. */
    constexpr Vector Lerp(const Vector& vector, const float t) const
    {
        return {M::Lerp(x, vector.x, t), M::Lerp(y, vector.y, t)};
    }

    /** @brief Returns the distance to another vector. */
    constexpr float Distance(const Vector& vector) const
    {
        return (*this - vector).Length();
    }

    /** @brief Determines whether two non-zero vectors are parallel. */
    constexpr bool IsParallelTo(const Vector& vector) const
    {
        if (NearlyEquals(Zero()) || vector.NearlyEquals(Zero()))
        {
            return false;
        }

        return std::abs(x * vector.y - y * vector.x) < EPSILON;
    }

    /** @brief Determines whether two non-zero vectors are perpendicular. */
    constexpr bool IsPerpendicularTo(const Vector& vector) const
    {
        if (NearlyEquals(Zero()) || vector.NearlyEquals(Zero()))
        {
            return false;
        }

        return M::NearlyEquals(std::abs(Dot(vector)), 0);
    }

    /** @brief Projects the vector stereographically onto a line. */
    constexpr float StereoProject() const
    {
        const float r = Length();

        return r * x / (r - y);
    }

    /** @brief Converts the vector to polar coordinates. */
    constexpr Polar ToPolar() const
    {
        return {Angle(), Length()};
    }

    /** @brief Compares the vector against another using an error tolerance. */
    constexpr bool NearlyEquals(const Vector& vector, const float epsilon = EPSILON) const
    {
        return M::NearlyEquals(x, vector.x, epsilon) && M::NearlyEquals(y, vector.y, epsilon);
    }

    /** @brief Adds another vector component-wise. */
    constexpr Vector operator+(const Vector& vector) const
    {
        return {x + vector.x, y + vector.y};
    }

    /** @brief Subtracts another vector component-wise. */
    constexpr Vector operator-(const Vector& vector) const
    {
        return {x - vector.x, y - vector.y};
    }

    /** @brief Multiplies another vector component-wise. */
    constexpr Vector operator*(const Vector& vector) const
    {
        return {x * vector.x, y * vector.y};
    }

    /** @brief Adds another vector to this vector. */
    constexpr Vector& operator+=(const Vector& vector)
    {
        return *this = *this + vector;
    }

    /** @brief Subtracts another vector from this vector. */
    constexpr Vector& operator-=(const Vector& vector)
    {
        return *this = *this - vector;
    }

    /** @brief Multiplies this vector component-wise by another vector. */
    constexpr Vector& operator*=(const Vector& vector)
    {
        return *this = *this * vector;
    }

    /** @brief Adds a scalar to every component. */
    constexpr Vector operator+(const float scalar) const
    {
        return {x + scalar, y + scalar};
    }

    /** @brief Subtracts a scalar from every component. */
    constexpr Vector operator-(const float scalar) const
    {
        return {x - scalar, y - scalar};
    }

    /** @brief Multiplies every component by a scalar. */
    constexpr Vector operator*(const float scalar) const
    {
        return {x * scalar, y * scalar};
    }

    /** @brief Divides every component by a scalar. */
    constexpr Vector operator/(const float scalar) const
    {
        return {x / scalar, y / scalar};
    }

    /** @brief Adds a scalar to every component in place. */
    constexpr Vector& operator+=(const float scalar)
    {
        return *this = *this + scalar;
    }

    /** @brief Subtracts a scalar from every component in place. */
    constexpr Vector& operator-=(const float scalar)
    {
        return *this = *this - scalar;
    }

    /** @brief Multiplies every component by a scalar in place. */
    constexpr Vector& operator*=(const float scalar)
    {
        return *this = *this * scalar;
    }

    /** @brief Divides every component by a scalar in place. */
    constexpr Vector& operator/=(const float scalar)
    {
        return *this = *this / scalar;
    }

    /** @brief Returns the negated vector. */
    constexpr Vector operator-() const
    {
        return -1 * *this;
    }

    /** @brief Compares two vectors for exact equality. */
    constexpr bool operator==(const Vector& vector) const
    {
        return x == vector.x && y == vector.y;
    }

    /** @brief Compares two vectors for inequality. */
    constexpr bool operator!=(const Vector& vector) const
    {
        return !(*this == vector);
    }

    /** @brief Adds a scalar to every component. */
    friend constexpr Vector operator+(const float scalar, const Vector& vector)
    {
        return vector + scalar;
    }

    /** @brief Subtracts every vector component from a scalar. */
    friend constexpr Vector operator-(const float scalar, const Vector& vector)
    {
        return {scalar - vector.x, scalar - vector.y};
    }

    /** @brief Multiplies every component by a scalar. */
    friend constexpr Vector operator*(const float scalar, const Vector& vector)
    {
        return vector * scalar;
    }

    /** @brief Divides a scalar by every vector component. */
    friend constexpr Vector operator/(const float scalar, const Vector& vector)
    {
        return {scalar / vector.x, scalar / vector.y};
    }

    /** @brief Writes the vector to an output stream. */
    friend std::ostream& operator<<(std::ostream& os, const Vector& vector)
    {
        os << "(" << vector.x << ", " << vector.y << ")";

        return os;
    }
};
} // namespace N::M