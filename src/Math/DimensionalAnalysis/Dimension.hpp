#pragma once
#include "FundamentalDimensionals.hpp"
#include "OperationalDimensionals.hpp"

namespace N::M
{

/** @brief Base type for physical quantities. */
struct IDimension
{
};

/** @brief Identifies types that represent physical quantities. */
template <typename T>
concept IsDimension = std::derived_from<T, IDimension>;

/**
 * @brief Stores a value together with its compile-time dimensional type.
 *
 * Arithmetic operators preserve dimensional correctness at compile time.
 * Operations on the same dimensional family combine their exponents, while
 * operations on different dimensions construct an operational dimensional type.
 *
 * @tparam ValueType The type used to store the numerical value.
 * @tparam D The compile-time dimensional representation of the value.
 */
template <typename ValueType, typename D> requires IsDimensional<D>
struct Dimension : IDimension
{
    //TODO- a better system would be to remove division entirely and define it as a x b^-1 instead.
    // cuz rn the system thinks a x b^-1 and a/b are different. will also make printing easier.
    // it will unify operations overall.

    //TODO- also some things are not fundamental dimensionals but have their own unit( wut i print).
    // ex: force, its mass / acceleration, but its represented with Newton (N).

    ValueType Value;

    /** @brief Constructs a dimension from a numerical value. */
    Dimension(const ValueType& value) : Value(value) {}

    /** @brief Adds two quantities with the same dimensional type. */
    Dimension operator+(const Dimension& other)
    {
        return Value + other.Value;
    }

    /** @brief Subtracts two quantities with the same dimensional type. */
    Dimension operator-(const Dimension& other)
    {
        return Value - other.Value;
    }

    /**
     * @brief Multiplies quantities from the same dimensional family.
     *
     * The exponent of the left-hand dimension is increased by the exponent
     * of the right-hand dimension.
     *
     * For example, m² * m³ produces m⁵.
     *
     * @tparam E The exponent of the right-hand dimension.
     */
    template <int E>
    Dimension<ValueType, typename D::template WithExponent<D::Exponent + E>> operator*(
        const Dimension<ValueType, typename D::template WithExponent<E>>& other)
    {
        return Value * other.Value;
    }

    /**
     * @brief Divides quantities from the same dimensional family.
     *
     * The exponent of the left-hand dimension is reduced by the exponent
     * of the right-hand dimension.
     *
     * For example, m³ / m produces m².
     *
     * @tparam E The exponent of the right-hand dimension.
     */
    template <int E>
    Dimension<ValueType, typename D::template WithExponent<D::Exponent - E>> operator/(
        const Dimension<ValueType, typename D::template WithExponent<E>>& other)
    {
        return Value / other.Value;
    }

    /**
     * @brief Divides quantities with different dimensional representations.
     *
     * The resulting dimension is represented as a compile-time division
     * expression rather than combining exponents.
     *
     * For example, m³ / s produces m³/s.
     *
     * @tparam Dim The dimensional type of the right-hand quantity.
     */
    template <typename Dim> requires IsDimensional<Dim>
    Dimension<ValueType, DivideDimensional<D, Dim>> operator/(const Dimension<ValueType, Dim>& other)
    {
        return Value / other.Value;
    }

    /**
     * @brief Multiplies quantities with different dimensional representations.
     *
     * The resulting dimension is represented as a compile-time multiplication
     * expression rather than combining exponents.
     *
     * For example, m² * s produces m²·s.
     *
     * @tparam Dim The dimensional type of the right-hand quantity.
     */
    template <typename Dim> requires IsDimensional<Dim>
    Dimension<ValueType, MultiplyDimensional<D, Dim>> operator*(const Dimension<ValueType, Dim>& other)
    {
        return Value * other.Value;
    }

    /** @brief Prints the numerical value followed by its dimensional representation. */
    friend std::ostream& operator<<(std::ostream& os, const Dimension& dimension)
    {
        os << dimension.Value << ' ';
        return D::Print(os);
    }

    ValueType& operator()()
    {
        return Value;
    }

    const ValueType& operator()() const
    {
        return Value;
    }
};

} // namespace N::M