#pragma once
namespace N::M
{
template <int Components> struct Vector
{
    constexpr Vector() {}
    constexpr explicit Vector(float all)
    {
        m_Data.fill(all);
    }

    constexpr const std::array<float, Components>& Data() const
    {
        return m_Data;
    }

    template <typename... Numbers>
    requires(sizeof...(Numbers) == Components && (std::convertible_to<Numbers, float> && ...))
    constexpr Vector(Numbers... numbers) : m_Data{static_cast<float>(numbers)...}
    {
    }

    constexpr float& operator()(const unsigned int component)
    {
        return m_Data[component];
    }

    constexpr const float& operator()(const unsigned int component) const
    {
        return m_Data[component];
    }

    static constexpr Vector Zero()
    {
        return Vector{0};
    }

    /** @brief Returns the squared length of the vector. */
    constexpr float LengthSquared() const
    {
        float result = 0;
        for (float& fl : m_Data)
        {
            result += fl * fl;
        }
        return result;
    }

    /** @brief Returns the length of the vector. */
    constexpr float Length() const
    {
        return std::sqrt(LengthSquared());
    }

    /** @brief Returns a normalized copy of the vector. */
    constexpr Vector Normalized() const
    {
        const float length = Length();

        if (length == 0)
        {
            return Zero();
        }

        Vector result;
        for (int i = 0; i < Components; ++i)
        {
            result(i) = (*this)(i) / length;
        }

        return result;
    }

    /** @brief Returns the dot product with another vector. */
    constexpr float Dot(const Vector& vector) const
    {
        float result = 0;
        for (int i = 0; i < Components; ++i)
        {
            result += (*this)(i)*vector(i);
        }

        return result;
    }

    /** @brief Linearly interpolates between this vector and another vector. */
    constexpr Vector Lerp(const Vector& vector, const float t) const
    {
        Vector result = 0;
        for (int i = 0; i < Components; ++i)
        {
            result(i) = M::Lerp((*this)(i), vector(i), t);
        }
        return result;
    }

    /** @brief Returns the distance to another vector. */
    constexpr float Distance(const Vector& vector) const
    {
        return (*this - vector).Length();
    }

    /** @brief Projects the vector stereographically onto a line. */
    constexpr Vector<Components - 1> StereoProject() const
    {
        const float r = Length();
        Vector<Components - 1> result;

        for (int i = 0; i < Components - 1; ++i)
        {
            result(i) = r * (*this)(i) / (r - (*this)(Components - 1));
        }

        return result;
    }

    /** @brief Compares the vector against another using an error tolerance. */
    constexpr bool NearlyEquals(const Vector& vector, const float epsilon = EPSILON) const
    {
        for (int i = 0; i < Components; ++i)
        {
            if (!M::NearlyEquals((*this)(i), vector(i), epsilon))
            {
                return false;
            }
        }
        return true;
    }

    friend std::ostream& operator<<(std::ostream& os, const Vector& vec)
    {
        os << "(";

        for (int i = 0; i < Components; ++i)
        {
            os << vec.m_Data[i];
            if (i != Components - 1)
            {
                os << ", ";
            }
        }
        os << ")";

        return os;
    }

  private:
    std::array<float, Components> m_Data{0};
};

} // namespace N::M