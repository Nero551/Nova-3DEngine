#pragma once
#include "Core/OuterCore/ECS/Component.hpp"
#include "Math/Functions/Function.hpp"
#include "Math/Matrix/Matrix3.hpp"
#include "Utilities/Log.hpp"

#include <concepts>
namespace Sketch
{

//TODO- pools/freelist for generating ids.
//
//TODO- the size of transform component is whats bottlenecking.
// split it. atleast split transform from the matrices (model matrix, normal matrix)

//TODO- add operator<< to all custom data structures. clean up data structure code.

//TODO- add operator[] for data structures instead of GetUnchecked.

template <typename T> struct Traits
{
    static constexpr bool IsComponent = std::derived_from<T, N::Component>;
};

struct Block : N::Component
{
};

struct NotBlock
{
};

using Index = unsigned int;

template <typename T, Index Size> struct Array
{
    bool Contains(Index index)
    {
        return index < Size;
    }

    T& At(Index index)
    {
        if (Contains(index))
        {
            return m_Data[index];
        }
        N::U::Log::Fatal("Array: Out of bounds.");
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

//TODO- make matrix and vector operators.
// scalar matrix, scalar vector, matrix matrix, vector vector & matrix vector operations.

template <unsigned int... Dimensions> struct Tensor
{
    static constexpr unsigned int Order = sizeof...(Dimensions);
    static constexpr unsigned int Size = (Dimensions * ...);

    Tensor() {}
    explicit Tensor(float all)
    {
        m_Data.fill(all);
    }

    template <typename... Numbers>
    requires(sizeof...(Numbers) == Size && (std::convertible_to<Numbers, float> && ...))
    Tensor(Numbers... numbers) : m_Data{static_cast<float>(numbers)...}
    {
    }

    template <typename... Indices>
    float& operator()(Indices... indices)
        requires(sizeof...(Indices) == Order && (std::integral<Indices> && ...))
    {
        std::array<unsigned int, Order> indexArray{static_cast<unsigned int>(indices)...};
        unsigned int flatIndex = Summation(0, Order - 1,
            N::M::Function<int, unsigned int>{[&](const int i) { return indexArray[i] * Strides[i]; }});

        N::U::Log::Info(flatIndex);
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
        unsigned int indent)
    {
        os << std::string(indent, ' ') << "[\n";

        if (dimension == Order - 1)
        {
            for (unsigned int i = 0; i < dimensions[dimension]; ++i)
            {
                os << std::string(indent + 4, ' ') << tensor.m_Data[flatIndex + i];

                if (i + 1 < dimensions[dimension])
                    os << ' ';
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
            os << '\n';
    }
};

template <unsigned int Row, unsigned int Column> struct Matrix
{
    static constexpr unsigned int Size = Row * Column;

    Matrix() {}

    explicit Matrix(float all)
    {
        m_Data.fill(all);
    }

    template <typename... Numbers>
    requires(sizeof...(Numbers) == Size && (std::convertible_to<Numbers, float> && ...))
    Matrix(Numbers... numbers) : m_Data{static_cast<float>(numbers)...}
    {
    }

    float& operator()(const int row, const int col) requires(row < Row && col < Column)
    {
        return m_Data[row][col];
    }

    std::array<std::array<float, Column>, Row>& Data()
    {
        return m_Data;
    }
    void RotateZ() requires(Row == Column == 3) {}
    void RotateZ() requires(Row == Column == 4) {}

  private:
    std::array<std::array<float, Column>, Row> m_Data;
};

template <int Components> struct Vector
{
    Vector() {}
    explicit Vector(float all)
    {
        m_Data.fill(all);
    }

    template <typename... Numbers>
    requires(sizeof...(Numbers) == Components && (std::convertible_to<Numbers, float> && ...))
    Vector(Numbers... numbers) : m_Data{static_cast<float>(numbers)...}
    {
    }

    float& operator()(const unsigned int component) requires(component < Components)
    {
        return m_Data[component];
    }

  private:
    std::array<float, Components> m_Data{0};
};

inline void Test()
{
    Tensor<2, 2> t(5, 2, 3, 6);
    t(0, 1) = 5;
    N::U::Log::Info(t);
}

} // namespace Sketch