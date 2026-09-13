#pragma once
namespace N::M
{

/** @brief Converts an integer exponent to its Unicode superscript representation. */
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
};

/** @brief Base type for compile-time dimensional representations. */
struct IDimensional
{
};

/** @brief Identifies types that represent compile-time dimensional representations. */
template <typename T>
concept IsDimensional = std::derived_from<T, IDimensional>;

/**
 * @brief Defines a fundamental dimension parameterized by an integer exponent.
 *
 * Each fundamental dimension is represented by a template such as Length<2>
 * or Time<-1>. WithExponent allows operations to produce the same dimension
 * with a different exponent while preserving its underlying type.
 *
 * @tparam Derived The fundamental dimension template.
 * @tparam Exp The dimension's exponent.
 */
template <template <int> typename Derived, int Exp> struct FundamentalDimensional : IDimensional
{
    static constexpr int Exponent = Exp;

    template <int E> using WithExponent = Derived<E>;
};

/** @brief Represents time with a compile-time exponent. */
template <int Exp> struct Time : FundamentalDimensional<Time, Exp>
{
    static std::ostream& Print(std::ostream& os)
    {
        return os << "s" << Superscript(Exp);
    }
};

/** @brief Represents length with a compile-time exponent. */
template <int Exp> struct Length : FundamentalDimensional<Length, Exp>
{
    static std::ostream& Print(std::ostream& os)
    {
        return os << "m" << Superscript(Exp);
    }
};

/** @brief Represents mass with a compile-time exponent. */
template <int Exp> struct Mass : FundamentalDimensional<Mass, Exp>
{
    static std::ostream& Print(std::ostream& os)
    {
        return os << "kg" << Superscript(Exp);
    }
};

} // namespace N::M