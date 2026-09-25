#pragma once

#include "Utilities/Log.hpp"

namespace N::U
{

/**
 * @brief Indirect two-dimensional storage with dense iteration.
 *
 * Maps a pair of indices `(A, B)` to densely packed values. Lookup is O(1)
 * after row/column storage is allocated, while iteration traverses only
 * active values. Erasing uses swap-and-pop, so dense indices can change.
 */
template <typename T> struct Indirect2DVector
{
    using Index = unsigned int;

    static constexpr Index InvalidIndex = std::numeric_limits<Index>::max();

    struct Key
    {
        Index A;
        Index B;
    };

    template <bool Const> struct BasicIterator
    {
        using IndirectVectorType = std::conditional_t<Const, const Indirect2DVector, Indirect2DVector>;
        using ReturnType = std::conditional_t<Const, const T, T>;

        IndirectVectorType* IndirectVector;
        Index Index;

        BasicIterator& operator++()
        {
            ++Index;
            return *this;
        }

        ReturnType& operator*() const
        {
            return IndirectVector->m_Data[Index];
        }

        ReturnType* operator->() const
        {
            return &IndirectVector->m_Data[Index];
        }

        bool operator!=(const BasicIterator& other) const
        {
            return Index != other.Index;
        }

        bool operator==(const BasicIterator& other) const
        {
            return Index == other.Index;
        }
    };

    using Iterator = BasicIterator<false>;
    using ConstIterator = BasicIterator<true>;

    Iterator begin()
    {
        return {this, 0};
    }

    Iterator end()
    {
        return {this, Size()};
    }

    /** @brief Inserts a value if the key does not exist and returns the stored value. */
    T& Push(Index a, Index b, const T& value)
    {
        Index& index = ResizeLookup(a, b)[b];

        if (index == InvalidIndex)
        {
            index = static_cast<Index>(m_Data.size());
            m_Data.push_back(value);
            m_Indices.push_back({a, b});
        }

        return m_Data[index];
    }

    bool Contains(const Index a, const Index b) const
    {
        if (a >= m_Lookup.size())
        {
            return false;
        }

        const auto& row = m_Lookup[a];

        return b < row.size() && row[b] != InvalidIndex;
    }

    T& At(Index a, Index b)
    {
        if (!Contains(a, b))
        {
            U::Log::Fatal("Indirect2DVector: Index does not exist.");
        }

        return m_Data[m_Lookup[a][b]];
    }

    const T& At(Index a, Index b) const
    {
        if (!Contains(a, b))
        {
            U::Log::Fatal("Indirect2DVector: Index does not exist.");
        }

        return m_Data[m_Lookup[a][b]];
    }

    /** @brief Constructs a value if the key does not exist and returns its iterator. */
    template <typename... Args> Iterator Emplace(Index a, Index b, Args&&... args)
    {
        Index& index = ResizeLookup(a, b)[b];

        if (index == InvalidIndex)
        {
            index = static_cast<Index>(m_Data.size());
            m_Data.emplace_back(std::forward<Args>(args)...);
            m_Indices.push_back({a, b});
        }

        return {.IndirectVector = this, .Index = index};
    }

    Iterator Find(const Index a, const Index b)
    {
        if (a >= m_Lookup.size())
        {
            return end();
        }

        const auto& row = m_Lookup[a];

        if (b >= row.size())
        {
            return end();
        }

        const Index index = row[b];

        if (index == InvalidIndex)
        {
            return end();
        }

        return {.IndirectVector = this, .Index = index};
    }

    /**
     * @brief Removes a value using swap-and-pop.
     * The dense index of the last value may change as a result.
     */
    void Erase(Index a, Index b)
    {
        if (!Contains(a, b))
        {
            return;
        }

        const Index index = m_Lookup[a][b];
        const Index lastIndex = m_Data.size() - 1;

        if (index != lastIndex)
        {
            const Key lastKey = m_Indices.back();

            std::swap(m_Data[index], m_Data.back());
            std::swap(m_Indices[index], m_Indices.back());

            m_Lookup[lastKey.A][lastKey.B] = index;
        }

        m_Data.pop_back();
        m_Indices.pop_back();
        m_Lookup[a][b] = InvalidIndex;
    }

    void Reserve(Index count)
    {
        m_Data.reserve(count);
        m_Indices.reserve(count);
    }

    Index Size() const
    {
        return m_Data.size();
    }

  private:
    std::vector<std::vector<Index>> m_Lookup{};
    std::vector<T> m_Data{};
    std::vector<Key> m_Indices{};

    std::vector<Index>& ResizeLookup(Index a, Index b)
    {
        if (m_Lookup.size() <= a)
        {
            m_Lookup.resize(a + 1);
        }

        auto& row = m_Lookup[a];

        if (row.size() <= b)
        {
            row.resize(b + 1, InvalidIndex);
        }

        return row;
    }
};

} // namespace N::U