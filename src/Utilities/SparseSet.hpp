#pragma once
#include <vector>

namespace N
{

/**
 * @brief Dense storage with sparse index lookup.
 *
 * Provides O(1) insertion, lookup, and deletion while keeping
 * stored values densely packed.
 *
 * @tparam D The stored data type.
 */
template <typename D> struct SparseSet
{
    /** @brief Represents an invalid dense or sparse index. */
    static constexpr size_t InvalidIndex = std::numeric_limits<size_t>::max();

    /** @brief Index into the dense storage. */
    using DenseIndex = size_t;

    /** @brief Index into the sparse storage. */
    using SparseIndex = size_t;

    /** @brief Contains the actual values. */
    std::vector<D> Dense{};

    /** @brief Maps dense indices to their corresponding sparse indices. */
    std::vector<SparseIndex> Indices{};

    /** @brief Maps sparse indices to their corresponding dense indices. */
    std::vector<DenseIndex> Sparse{};

    /**
     * @brief Checks whether a sparse index is present.
     *
     * @param s Sparse index to check.
     * @return True if the sparse index exists, otherwise false.
     */
    bool Contains(const size_t s) const
    {
        return s < Sparse.size() && Sparse[s] != InvalidIndex;
    }

    /**
     * @brief Inserts a value at a sparse index.
     *
     * Does nothing if the sparse index already exists.
     *
     * @param s Sparse index of the value.
     * @param d Value to insert.
     * @return Reference to the stored value.
     */
    D& Push(const size_t s, const D& d)
    {
        if (!Contains(s))
        {
            if (s >= Sparse.size())
            {
                Sparse.resize(s + 1, InvalidIndex);
            }

            Sparse[s] = Dense.size();
            Indices.push_back(s);
            Dense.push_back(d);
        }

        return Dense[Sparse[s]];
    }

    /**
     * @brief Constructs and inserts a value at a sparse index.
     *
     * Does nothing if the sparse index already exists.
     *
     * @tparam Args Constructor argument types.
     * @param s Sparse index of the value.
     * @param args Arguments forwarded to the value constructor.
     * @return Reference to the stored value.
     */
    template <typename... Args> D& Emplace(const size_t s, Args&&... args)
    {
        if (!Contains(s))
        {
            if (s >= Sparse.size())
            {
                Sparse.resize(s + 1, InvalidIndex);
            }

            Sparse[s] = Dense.size();
            Indices.push_back(s);
            Dense.emplace_back(std::forward<Args>(args)...);
        }

        return Dense[Sparse[s]];
    }

    /**
     * @brief Retrieves the value at a sparse index.
     *
     * @param s Sparse index of the value.
     * @return Reference to the stored value.
     *
     * @note Logs a fatal error if the sparse index does not exist.
     */
    D& Get(const size_t s)
    {
        if (!Contains(s))
        {
            U::Logger::Fatal("SparseSet does not contain the specified sparse index.");
        }

        return Dense[Sparse[s]];
    }

    /**
     * @brief Removes the value at a sparse index.
     *
     * Uses swap-and-pop to keep the dense storage packed.
     *
     * @param s Sparse index of the value to remove.
     */
    void Delete(const size_t s)
    {
        if (!Contains(s))
            return;

        DenseIndex index = Sparse[s];

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

    /** @brief Returns the number of stored values. */
    size_t Size() const
    {
        return Dense.size();
    }

    /**
     * @brief Iterator over the dense storage.
     *
     * Dereferencing the iterator returns the sparse index and
     * corresponding dense value.
     */
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
        std::pair<size_t, D&> operator*() const
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
};

} // namespace N