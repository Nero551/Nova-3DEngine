#pragma once
namespace N::M
{

struct IDimensional
{
};

template <template <int> typename Derived, int Exp> struct Dimensional : IDimensional
{
    static constexpr int Exponent = Exp;

    template <int E> using WithExponent = Derived<E>;
};

template <int Exp> struct Time : Dimensional<Time, Exp>
{
    static constexpr std::string_view Name = "s";
};

template <int Exp> struct Length : Dimensional<Length, Exp>
{
    static constexpr std::string_view Name = "m";
};

template <int Exp> struct Mass : Dimensional<Mass, Exp>
{
    static constexpr std::string_view Name = "kg";
};

} // namespace N::M