#pragma once

#include "Utilities/Log.hpp"

#include <limits>
#include <utility>
#include <vector>

namespace N::U
{

template <typename T> struct Indirect2DVector
{
    using Index = unsigned int;

    static constexpr Index InvalidIndex = std::numeric_limits<Index>::max();

    struct Key
    {
        Index A;
        Index B;
    };

  private:
    std::vector<std::vector<Index>> m_Lookup{};
    std::vector<T> m_Data{};
    std::vector<Key> m_Indices{};

  public:
    struct Iterator
    {
        Indirect2DVector* IndirectVector;
        size_t Index;

        Iterator& operator++()
        {
            ++Index;
            return *this;
        }

        T& operator*() const
        {
            return IndirectVector->m_Data[Index];
        }

        T* operator->() const
        {
            return &IndirectVector->m_Data[Index];
        }

        bool operator!=(const Iterator& other) const
        {
            return Index != other.Index;
        }

        bool operator==(const Iterator& other) const
        {
            return Index == other.Index;
        }
    };

    Iterator begin()
    {
        return {this, 0};
    }

    Iterator end()
    {
        return {this, m_Data.size()};
    }

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

    T& Get(Index a, Index b)
    {
        if (!Contains(a, b))
        {
            U::Log::Fatal("Indirect2DVector: Index does not exist.");
        }

        return m_Data[m_Lookup[a][b]];
    }

    const T& Get(Index a, Index b) const
    {
        if (!Contains(a, b))
        {
            U::Log::Fatal("Indirect2DVector: Index does not exist.");
        }

        return m_Data[m_Lookup[a][b]];
    }

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

    Iterator Find(const size_t a, const size_t b)
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

    void Delete(Index a, Index b)
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

  private:
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