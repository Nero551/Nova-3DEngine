#pragma once

#include <vector>

namespace N
{

/**
 * @brief Dense storage with sparse index lookup.
 *
 * Provides O(1) insertion, lookup, and deletion while keeping values densely packed.
 *
 * @tparam D Stored data type.
 */
template <typename D> struct SparseSet
{
    using DenseIndex = size_t;
    using SparseIndex = size_t;

    /** @brief Represents an invalid index. */
    static constexpr size_t InvalidIndex = std::numeric_limits<size_t>::max();

    /** @brief Returns whether the specified sparse index exists. */
    bool Contains(const SparseIndex s) const
    {
        return s < Sparse.size() && Sparse[s] != InvalidIndex;
    }

    /** @brief Returns the value associated with the specified sparse index. */
    D& Get(const SparseIndex s)
    {
        if (!Contains(s))
            U::Logger::Fatal("SparseSet does not contain the specified sparse index.");

        return Dense[Sparse[s]];
    }

    /** @brief Returns the value associated with the specified dense index. */
    D& GetByIndex(const DenseIndex index)
    {
        if (index >= Dense.size())
            U::Logger::Fatal("SparseSet dense index out of bounds.");

        return Dense[index];
    }

    /** @brief Returns the sparse index associated with a dense index. */
    SparseIndex GetSparseIndex(const DenseIndex index) const
    {
        return Indices[index];
    }

    /** @brief Returns the dense index associated with a sparse index. */
    DenseIndex GetDenseIndex(const SparseIndex index) const
    {
        return Sparse[index];
    }

    /** @brief Inserts a value at the specified sparse index. */
    D& Push(const SparseIndex s, const D& d)
    {
        if (!Contains(s))
        {
            if (s >= Sparse.size())
                Sparse.resize(s + 1, InvalidIndex);

            Sparse[s] = Dense.size();
            Indices.push_back(s);
            Dense.push_back(d);
        }

        return Dense[Sparse[s]];
    }

    /** @brief Constructs a value at the specified sparse index. */
    template <typename... Args> D& Emplace(const SparseIndex s, Args&&... args)
    {
        if (!Contains(s))
        {
            if (s >= Sparse.size())
                Sparse.resize(s + 1, InvalidIndex);

            Sparse[s] = Dense.size();
            Indices.push_back(s);
            Dense.emplace_back(std::forward<Args>(args)...);
        }

        return Dense[Sparse[s]];
    }

    /** @brief Removes the value at the specified sparse index. */
    void Delete(const SparseIndex s)
    {
        if (!Contains(s))
            return;

        const DenseIndex index = Sparse[s];

        if (index != Dense.size() - 1)
        {
            const SparseIndex lastElement = Indices.back();

            std::swap(Dense[index], Dense.back());
            std::swap(Indices[index], Indices.back());

            Sparse[lastElement] = index;
        }

        Dense.pop_back();
        Indices.pop_back();
        Sparse[s] = InvalidIndex;
    }

    /** @brief Removes all values while retaining allocated memory. */
    void Clear()
    {
        Dense.clear();
        Indices.clear();
        Sparse.clear();
    }

    /** @brief Reserves dense storage for the specified number of elements. */
    void Reserve(const size_t size)
    {
        Dense.reserve(size);
        Indices.reserve(size);
    }

    /** @brief Returns the number of stored values. */
    size_t Size() const
    {
        return Dense.size();
    }

    /** @brief Returns whether the set contains no values. */
    bool Empty() const
    {
        return Dense.empty();
    }

    struct Iterator
    {
        SparseSet& Set;
        DenseIndex Index;

        /** @brief Advances the iterator to the next value. */
        Iterator& operator++()
        {
            ++Index;
            return *this;
        }

        /** @brief Returns the sparse index and corresponding value. */
        std::pair<SparseIndex, D&> operator*() const
        {
            return {Set.Indices[Index], Set.Dense[Index]};
        }

        /** @brief Compares two iterators for inequality. */
        bool operator!=(const Iterator& other) const
        {
            return Index != other.Index;
        }
    };

    /** @brief Returns an iterator to the first value. */
    Iterator begin()
    {
        return {*this, 0};
    }

    /** @brief Returns an iterator past the last value. */
    Iterator end()
    {
        return {*this, Size()};
    }

  private:
    /** @brief Contains the actual values. */
    std::vector<D> Dense{};

    /** @brief Maps dense indices to their sparse indices. */
    std::vector<SparseIndex> Indices{};

    /** @brief Maps sparse indices to their dense indices. */
    std::vector<DenseIndex> Sparse{};
};

} // namespace N