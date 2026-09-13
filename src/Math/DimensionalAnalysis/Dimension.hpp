#pragma once
#include "Dimensionals.hpp"

namespace N::M
{

struct IDimension
{
};

template <typename T>
concept IsDimension = std::derived_from<T, IDimension>;

template <typename ValueType, typename D> requires IsDimensional<D>
struct Dimension : IDimension
{
    ValueType Value;

    Dimension(const ValueType& value) : Value(value) {}

    Dimension operator+(const Dimension& other)
    {
        return Value + other.Value;
    }

    Dimension operator-(const Dimension& other)
    {
        return Value - other.Value;
    }

    template <int E>
    Dimension<ValueType, typename D::template WithExponent<D::Exponent + E>> operator*(
        const Dimension<ValueType, typename D::template WithExponent<E>>& other)
    {
        return Value * other.Value;
    }

    template <int E>
    Dimension<ValueType, typename D::template WithExponent<D::Exponent - E>> operator/(
        const Dimension<ValueType, typename D::template WithExponent<E>>& other)
    {
        return Value / other.Value;
    }

    template <typename Dim> requires IsDimensional<Dim>
    Dimension<ValueType, DivideDimensional<D, Dim>> operator/(const Dimension<ValueType, Dim>& other)
    {
        return Value / other.Value;
    }

    template <typename Dim> requires IsDimensional<Dim>
    Dimension<ValueType, MultiplyDimensional<D, Dim>> operator*(const Dimension<ValueType, Dim>& other)
    {
        return Value * other.Value;
    }

    friend std::ostream& operator<<(std::ostream& os, const Dimension& dimension)
    {
        os << dimension.Value << ' ';
        return D::Print(os);
    }
};

} // namespace N::M