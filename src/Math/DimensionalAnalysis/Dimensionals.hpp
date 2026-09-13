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

struct IDimensional
{
};

template <typename T>
concept IsDimensional = std::derived_from<T, IDimensional>;

template <template <int> typename Derived, int Exp> struct Dimensional : IDimensional
{
    static constexpr int Exponent = Exp;

    template <int E> using WithExponent = Derived<E>;
};

template <int Exp> struct Time : Dimensional<Time, Exp>
{
    static constexpr std::string_view Name = "s";

    static std::ostream& Print(std::ostream& os)
    {
        return os << Name << Superscript(Exp);
    }
};

template <int Exp> struct Length : Dimensional<Length, Exp>
{
    static constexpr std::string_view Name = "m";

    static std::ostream& Print(std::ostream& os)
    {
        return os << Name << Superscript(Exp);
    }
};

template <int Exp> struct Mass : Dimensional<Mass, Exp>
{
    static constexpr std::string_view Name = "kg";

    static std::ostream& Print(std::ostream& os)
    {
        return os << Name << Superscript(Exp);
    }
};

struct OperationalDimensional : IDimensional
{
    template <int E> using WithExponent = OperationalDimensional;
    static constexpr int Exponent = 0;
};
template <typename A, typename B> requires(IsDimensional<A> && IsDimensional<B>)
struct DivideDimensional : OperationalDimensional
{
    using Numerator = A;
    using Denominator = B;

    static std::ostream& Print(std::ostream& os)
    {
        Numerator::Print(os);
        os << '/';
        return Denominator::Print(os);
    }
};

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