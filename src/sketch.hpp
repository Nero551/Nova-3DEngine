#pragma once
#include "Core/OuterCore/ECS/Component.hpp"
#include "Math/Functions/Function.hpp"
#include "Math/Matrix/Matrix3.hpp"
#include "Math/Vector/Vector.hpp"
#include "Math/Vector/Vector4.hpp"
#include "Utilities/Log.hpp"

namespace Sketch
{

//TODO- the size of transform component is whats bottlenecking.
// split it. atleast split transform from the matrices (model matrix, normal matrix)

//TODO- add operator<< to all custom data structures. clean up datHa structure code.

//TODO- add vector operators. for vector.
//
//TODO- If converting a general Quaternion to a rotation quaternion proves
// expensive in a hot path, introduce a specialized RotQuaternion (RQuaternion)
// type and explicit conversion between the two. it will just be a unit quaternion with half angle representation.

//TODO- remove matrices from transform component
// make render batches store sparse sets that map entity ids to normal/model matrices.
// and only recompute matrices when global transform.IsChanged.

//TODO- play minecraft in the redstone modpack i made for understanding logic gates.

//TODO- rework dimensional analysis system

using Index = unsigned int;
template <typename T, Index Size> struct Array
{
    static bool Contains(const Index index)
    {
        return index < Size;
    }

    T& At(Index index)
    {
        if (!Contains(index))
        {
            N::U::Log::Fatal("Array: Out of bounds.");
        }
        return m_Data[index];
    }

    T& operator[](Index index)
    {
        return m_Data[index];
    }

  private:
    T m_Data[Size];
};

template <typename Input, typename Output>
Output Summation(const int start, const int end, const N::M::Function<Input, Output>& f)
    requires(std::is_arithmetic_v<Input>)
{
    Output result{};

    for (int i = start; i <= end; ++i)
    {
        result += f(static_cast<Input>(i));
    }

    return result;
}

template <unsigned int... Dimensions> struct Tensor
{
    static constexpr unsigned int Order = sizeof...(Dimensions);
    static constexpr unsigned int Size = (Dimensions * ...);

    constexpr Tensor() {}
    constexpr explicit Tensor(float all)
    {
        m_Data.fill(all);
    }

    template <typename... Numbers>
    requires(sizeof...(Numbers) == Size && (std::convertible_to<Numbers, float> && ...))
    constexpr Tensor(Numbers... numbers) : m_Data{static_cast<float>(numbers)...}
    {
    }

    template <typename... Indices>
    constexpr float& operator()(Indices... indices)
        requires(sizeof...(Indices) == Order && (std::integral<Indices> && ...))
    {
        std::array<unsigned int, Order> indexArray{static_cast<unsigned int>(indices)...};
        unsigned int flatIndex = Summation(0, Order - 1,
            N::M::Function<int, unsigned int>{[&](const int i) { return indexArray[i] * Strides[i]; }});

        return m_Data[flatIndex];
    }

    friend std::ostream& operator<<(std::ostream& os, const Tensor& tensor)
    {
        Print(os, tensor, {Dimensions...}, 0, 0, 0);
        return os;
    }

  private:
    std::array<float, Size> m_Data{0};

    static constexpr std::array<unsigned int, Order> GetStrides()
    {
        constexpr std::array<unsigned int, Order> dimensions = {Dimensions...};
        std::array<unsigned int, Order> strides{};
        unsigned int stride = 1;

        for (int i = Order - 1; i >= 0; --i)
        {
            strides[i] = stride;
            stride *= dimensions[i];
        }

        return strides;
    }

    static constexpr std::array<unsigned int, Order> Strides = GetStrides();

    static void Print(std::ostream& os, const Tensor& tensor,
        const std::array<unsigned int, Order> dimensions, unsigned int dimension, unsigned int flatIndex,
        const unsigned int indent)
    {
        os << std::string(indent, ' ') << "[\n";

        if (dimension == Order - 1)
        {
            for (unsigned int i = 0; i < dimensions[dimension]; ++i)
            {
                os << std::string(indent + 4, ' ') << tensor.m_Data[flatIndex + i];

                if (i + 1 < dimensions[dimension])
                {
                    os << ' ';
                }
            }

            os << '\n';
        }
        else
        {
            for (unsigned int i = 0; i < dimensions[dimension]; ++i)
            {
                Print(os, tensor, dimensions, dimension + 1, flatIndex + i * tensor.Strides[dimension],
                    indent + 4);
            }
        }

        os << std::string(indent, ' ') << ']';

        if (dimension != 0)
        {
            os << '\n';
        }
    }
};

inline std::string Superscript(int exponent)
{
    static constexpr std::string_view Digits[] = {"⁰", "¹", "²", "³", "⁴", "⁵", "⁶", "⁷", "⁸", "⁹"};
    if (exponent == 1)
    {
        return "";
    }

    std::string result;

    if (exponent < 0)
    {
        result += "⁻";
        exponent = -exponent;
    }

    std::string digits = std::to_string(exponent);

    for (char digit : digits)
    {
        result += Digits[digit - '0'];
    }

    return result;
};

struct IDimensional
{
};

struct IOperationDimensional : IDimensional
{
};

template <template <int> typename Derived, int Exp> struct Dimensional : IDimensional
{
    static constexpr int Exponent = Exp;
    template <int E> using WithExponent = Derived<E>;
    using Normalized = Derived<Exp>;
};

template <typename A, typename B> struct OperationDimensional : IOperationDimensional
{
    using Left = A::Normalized;
    using Right = B::Normalized;

    using Normalized = std::conditional_t<
        std::same_as<typename Left::template WithExponent<1>, typename Right::template WithExponent<1>>,
        typename Left::template WithExponent<Left::Exponent + Right::Exponent>,
        OperationDimensional<Left, Right>>;

    static std::ostream& Print(std::ostream& os)
    {
        if (Left::Exponent < 0 && Right::Exponent < 0)
        {
            os << "1/(";
            Left::template WithExponent<-Left::Exponent>::Print(os);
            Right::template WithExponent<-Right::Exponent>::Print(os);
            os << ")";
        }

        if (Left::Exponent < 0 && Right::Exponent > 0)
        {
            Right::Print(os);
            os << "/";
            Left::template WithExponent<-Left::Exponent>::Print(os);
        }

        if (Left::Exponent > 0 && Right::Exponent < 0)
        {
            Left::Print(os);
            os << "/";
            Right::template WithExponent<-Right::Exponent>::Print(os);
        }

        if (Left::Exponent > 0 && Right::Exponent > 0)
        {
            Left::Print(os);
            Right::Print(os);
        }

        return os;
    }

    template <int E> using WithExponent = OperationDimensional;
    static constexpr int Exponent = 1;
};

template <int Exp> struct Time : Dimensional<Time, Exp>
{
    static std::ostream& Print(std::ostream& os)
    {
        return os << "s" << Superscript(Exp);
    }
};
template <int Exp> struct Length : Dimensional<Length, Exp>
{
    static std::ostream& Print(std::ostream& os)
    {
        return os << "m" << Superscript(Exp);
    }
};

template <typename T, typename D> requires(std::derived_from<D, IDimensional>)
struct Dimension
{
    T Value{0};

    constexpr Dimension() {}
    constexpr Dimension(const T& value) : Value(value) {}

    template <typename O>
    constexpr Dimension operator+(const Dimension<T, O>& other)
        requires(std::same_as<typename O::Normalized, typename D::Normalized>)
    {
        return {Value + other.Value};
    }

    template <typename O>
    constexpr Dimension operator-(const Dimension<T, O>& other)
        requires(std::same_as<typename O::Normalized, typename D::Normalized>)
    {
        return {Value - other.Value};
    }

    template <int E>
    constexpr Dimension<T, typename D::template WithExponent<D::Exponent + E>> operator*(
        const Dimension<T, typename D::template WithExponent<E>>& other)
    {
        return {Value * other.Value};
    }

    template <int E>
    constexpr Dimension<T, typename D::template WithExponent<D::Exponent - E>> operator/(
        const Dimension<T, typename D::template WithExponent<E>>& other)
    {
        return {Value / other.Value};
    }

    template <typename O> constexpr Dimension<T, OperationDimensional<D, O>> operator*(Dimension<T, O>& other)
    {
        return {Value * other.Value};
    }

    template <typename O>
    constexpr Dimension<T, OperationDimensional<D, typename O::template WithExponent<-O::Exponent>>>
    operator/(Dimension<T, O>& other)
    {
        return {Value / other.Value};
    }

    constexpr T& operator()()
    {
        return Value;
    }
    constexpr const T& operator()() const
    {
        return Value;
    }

    friend std::ostream& operator<<(std::ostream& os, Dimension dimension)
    {
        os << dimension.Value << ' ';
        return D::Normalized::Print(os);
    }
};

inline void Test()
{
    //TODO- to fix this, add a normalize constexpr in operationDimensional.
    // make operators use the Normalized version of the operational dimensional.
    // so nested operations will work
    // make ALL operators use the Normalized version of a dimensional.
    // probably want normal Dimensional to have this as well, its normalized is just itself.
    //
    // using Velocity = OperationDimensional<Length<1>, Time<-1>>;
    // using Acceleration = OperationDimensional<Velocity, Time<-1>>;
    // Dimension<float, OperationDimensional<Length<2>, Length<1>>> a;
    // Dimension<float, Length<3>> l2;
    //
    // Dimension<float, OperationDimensional<Length<1>, Time<-2>>> t{5};
    // N::U::Log::Info(a + l2);

    N::M::Vector<4> v;
}

} // namespace Sketch