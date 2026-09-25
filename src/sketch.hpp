#pragma once
#include "Core/OuterCore/ECS/Component.hpp"
#include "Math/Functions/Function.hpp"
#include "Math/Matrix/Matrix3.hpp"
#include "Utilities/Log.hpp"

#include <concepts>
namespace Sketch
{

//TODO- the size of transform component is whats bottlenecking.
// split it. atleast split transform from the matrices (model matrix, normal matrix)

//TODO- add operator<< to all custom data structures. clean up data structure code.

//TODO- add operator[] for data structures instead of GetUnchecked.

//TODO- add vector operators. for vector.

//TODO- remove matrices from transform component
// make render batches store sparse sets that map entity ids to normal/model matrices.
// and only recompute matrices when global transform.IsChanged.

//TODO- If converting a general Quaternion to a rotation quaternion proves
// expensive in a hot path, introduce a specialized RotQuaternion (RQuaternion)
// type and explicit conversion between the two. it will just be a unit quaternion with half angle representation.

//TODO- make entities use GIndexPool instead of IndexPool.

using Index = unsigned int;
template <typename T, Index Size> struct Array
{
    static bool Contains(Index index)
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

template <int Components> struct Vector
{
    constexpr Vector() {}
    constexpr explicit Vector(float all)
    {
        m_Data.fill(all);
    }

    template <typename... Numbers>
    requires(sizeof...(Numbers) == Components && (std::convertible_to<Numbers, float> && ...))
    constexpr Vector(Numbers... numbers) : m_Data{static_cast<float>(numbers)...}
    {
    }

    float& operator()(const unsigned int component)
    {
        return m_Data[component];
    }

    const float& operator()(const unsigned int component) const
    {
        return m_Data[component];
    }

    inline static const Vector Zero{0};

  private:
    std::array<float, Components> m_Data{0};
};

template <unsigned int Row, unsigned int Column> struct Matrix
{
    static constexpr unsigned int Size = Row * Column;

    constexpr Matrix() {}

    constexpr explicit Matrix(float all)
    {
        for (auto& row : m_Data)
        {
            row.fill(all);
        };
    }

    template <typename... Numbers>
    requires(sizeof...(Numbers) == Size && (std::convertible_to<Numbers, float> && ...))
    constexpr Matrix(Numbers... numbers) : m_Data{static_cast<float>(numbers)...}
    {
    }

    float& operator()(const int row, const int col)
    {
        return m_Data[row][col];
    }

    const float& operator()(const int row, const int col) const
    {
        return m_Data[row][col];
    }

    std::array<std::array<float, Column>, Row>& Data()
    {
        return m_Data;
    }

    Matrix operator+(const Matrix& mat) const
    {
        Matrix result = Zero;

        for (int row = 0; row < Row; ++row)
        {
            for (int col = 0; col < Column; ++col)
            {
                result(row, col) = (*this)(row, col) + mat(row, col);
            }
        }

        return result;
    }

    Matrix operator-(const Matrix& mat) const
    {
        Matrix result = Zero;

        for (int row = 0; row < Row; ++row)
        {
            for (int col = 0; col < Column; ++col)
            {
                result(row, col) = (*this)(row, col) - mat(row, col);
            }
        }

        return result;
    }

    template <unsigned int R, unsigned int C>
    Matrix<Row, C> operator*(const Matrix<R, C>& mat) const requires(Column == R)
    {
        Matrix<Row, C> result = Matrix<Row, C>::Zero;
        for (int row = 0; row < Row; ++row)
        {
            for (int col = 0; col < C; ++col)
            {
                result(row, col) = Summation(0, R - 1,
                    N::M::Function<int, float>{[&](const int k) { return (*this)(row, k) * mat(k, col); }});
            }
        }

        return result;
    }

    Vector<Row> operator*(const Vector<Column>& vec) const
    {
        Vector<Row> result = Vector<Row>::Zero;

        for (int row = 0; row < Row; ++row)
        {
            for (int col = 0; col < Column; ++col)
            {
                result(row) += (*this)(row, col) * vec(col);
            }
        }

        return result;
    }

    Matrix operator*(float scalar) const
    {
        Matrix result = Zero;

        for (int row = 0; row < Row; ++row)
        {
            for (int col = 0; col < Column; ++col)
            {
                result(row, col) = (*this)(row, col) * scalar;
            }
        }

        return result;
    }
    Matrix operator/(float scalar) const
    {
        Matrix result = Zero;

        for (int row = 0; row < Row; ++row)
        {
            for (int col = 0; col < Column; ++col)
            {
                result(row, col) = (*this)(row, col) / scalar;
            }
        }

        return result;
    }

    friend std::ostream& operator<<(std::ostream& os, const Matrix& matrix)
    {
        for (unsigned int row = 0; row < Row; ++row)
        {
            os << "[ ";

            for (unsigned int col = 0; col < Column; ++col)
            {
                os << matrix(row, col);

                if (col + 1 < Column)
                    os << ", ";
            }

            os << " ]";

            if (row + 1 < Row)
                os << '\n';
        }

        return os;
    }

    void RotateZ() requires(Row == Column && Row == 3) {}
    void RotateZ() requires(Row == Column && Row == 4) {}

    inline static const Matrix Zero{0};

  private:
    std::array<std::array<float, Column>, Row> m_Data;
};

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
    float& operator()(Indices... indices)
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

inline void Test()
{
    Vector<10> v;
    Matrix<1000, 100> m1{5};
    Matrix<100, 500> m2{3};
    // Tensor<100, 100, 100,100> t; // dont do this, it blows up the stack and seg faults.
    // auto bigT = std::make_unique<Tensor<100, 100, 100, 100, 100, 100>>(5); // this is 40 GB of memory right there.
    //! tensors scale dangerously, a 100 10 dimensional tensor is 10^20 floats. thats more than the seconds since the big bang.

    Tensor<2, 1, 6> t2;
    N::U::Log::Print(m1 * m2);
}

} // namespace Sketch