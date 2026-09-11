#pragma once
#include "Utilities/Logger.hpp"

#include <vector>
namespace N
{

template <typename T> struct Indirect2DVector
{
    using Index = size_t;
    static constexpr Index InvalidIndex = std::numeric_limits<Index>::max();

    std::vector<std::vector<Index>> Lookup{};
    std::vector<T> Data{};
    T& Push(Index a, Index b, const T& value)
    {
        auto& row = ResizeLookup(a, b);

        if (row[b] == InvalidIndex)
        {
            const Index index = static_cast<Index>(Data.size());

            row[b] = index;
            Data.push_back(value);
        }

        return Data[row[b]];
    }

    bool Contains(const Index a, const Index b) const
    {
        if (a >= Lookup.size())
        {
            return false;
        }

        const auto& Row = Lookup[a];

        return b < Row.size() && Row[b] != InvalidIndex;
    }

    T& Get(Index a, Index b)
    {
        if (!Contains(a, b))
        {
            U::Logger::Fatal("Indirect2DVector: Index does not exist.");
        }

        return Data[Lookup[a][b]];
    }

    const T& Get(Index a, Index b) const
    {
        if (!Contains(a, b))
        {
            U::Logger::Fatal("Indirect2DVector: Index does not exist.");
        }

        return Data[Lookup[a][b]];
    }

    template <typename... Args> T& Emplace(Index a, Index b, Args&&... args)
    {
        auto& row = ResizeLookup(a, b);
        Index& index = row[b];

        if (index == InvalidIndex)
        {
            index = static_cast<Index>(Data.size());
            Data.emplace_back(std::forward<Args>(args)...);
        }

        return Data[index];
    }

    void Reserve(Index count)
    {
        Data.reserve(count);
    }

    struct Iterator
    {
        Indirect2DVector& IndirectVector;
        size_t Index;

        /** @brief Advances the iterator to the next value. */
        Iterator& operator++()
        {
            ++Index;
            return *this;
        }

        /** @brief Returns the sparse index and corresponding value. */
        T& operator*() const
        {
            return IndirectVector.Data[Index];
        }

        /** @brief Compares two iterators for inequality. */
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
        return {*this, 0};
    }

    Iterator end()
    {
        return {*this, Data.size()};
    }

  private:
    std::vector<Index>& ResizeLookup(Index a, Index b)
    {
        if (Lookup.size() <= a)
        {
            Lookup.resize(a + 1);
        }

        auto& row = Lookup[a];

        if (row.size() <= b)
        {
            row.resize(b + 1, InvalidIndex);
        }

        return row;
    }
};

} // namespace N