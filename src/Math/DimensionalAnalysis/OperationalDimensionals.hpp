#pragma once
#include "FundamentalDimensionals.hpp"

namespace N::M
{

/** @brief Base type for dimensional types produced by arithmetic operations. */
struct OperationalDimensional : IDimensional
{
    /**
     * @brief Provides a compatible exponent interface for operational dimensions.
     *
     * Operational dimensions do not represent a single fundamental dimension,
     * so changing their exponent leaves the operational expression unchanged.
     */
    template <int E> using WithExponent = OperationalDimensional;

    static constexpr int Exponent = 1;
};

/**
 * @brief Represents a compile-time dimensional division expression.
 *
 * The operands are retained as types, allowing compound dimensions to form
 * recursively. For example, Length<1> / Time<1> / Time<1> becomes a nested
 * DivideDimensional expression.
 *
 * @tparam A The numerator dimensional type.
 * @tparam B The denominator dimensional type.
 */
template <typename A, typename B> requires(IsDimensional<A> && IsDimensional<B>)
struct DivideDimensional : OperationalDimensional
{
    using Numerator = A;
    using Denominator = B;

    static std::ostream& Print(std::ostream& os)
    {
        if constexpr (std::derived_from<Numerator, OperationalDimensional>)
        {
        }

        Numerator::Print(os);
        os << '/';
        return Denominator::Print(os);
    }
};

/**
 * @brief Represents a compile-time dimensional multiplication expression.
 *
 * The operands are retained as types, allowing compound dimensions to form
 * recursively. For example, Length<2> * Time<-1> represents m²·s⁻¹.
 *
 * @tparam A The left-hand dimensional type.
 * @tparam B The right-hand dimensional type.
 */
template <typename A, typename B> requires(IsDimensional<A> && IsDimensional<B>)
struct MultiplyDimensional : OperationalDimensional
{
    using Multiplicand = A;
    using Multiplier = B;

    static std::ostream& Print(std::ostream& os)
    {
        Multiplicand::Print(os);
        os << "·";
        return Multiplier::Print(os);
    }
};

} // namespace N::M