#pragma once
namespace N::M
{
inline std::string Superscript(int exponent)
{
    static constexpr std::string_view Digits[] = {"⁰", "¹", "²", "³", "⁴", "⁵", "⁶", "⁷", "⁸", "⁹"};
    if (exponent == 1)
        return "";

    std::string result;

    if (exponent < 0)
    {
        result += "⁻";
        exponent = -exponent;
    }

    std::string digits = std::to_string(exponent);

    for (char digit : digits)
        result += Digits[digit - '0'];

    return result;
}

template <int Exp> struct Time
{
    static constexpr int Exponent = Exp;
    static constexpr std::string_view Name = "s";

    template <int E> using WithExponent = Time<E>;
};

template <typename ValueType, typename D> struct Dimension
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
    using DimensionExponentSub = Dimension<ValueType, typename D::template WithExponent<D::Exponent - E>>;

    template <int E>
    using DimensionExponentAdd = Dimension<ValueType, typename D::template WithExponent<D::Exponent + E>>;

    template <int E>
    DimensionExponentAdd<E> operator*(const Dimension<ValueType, typename D::template WithExponent<E>>& other)
    {
        return Value * other.Value;
    }

    template <int E>
    DimensionExponentSub<E> operator/(const Dimension<ValueType, typename D::template WithExponent<E>>& other)
    {
        return Value / other.Value;
    }

    friend std::ostream& operator<<(std::ostream& os, const Dimension& t)
    {
        os << t.Value;
        ((os << ' ' << D::Name << Superscript(D::Exponent)));

        return os;
    }
};

} // namespace N::M