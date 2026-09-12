#pragma once

#include <vector>

namespace N
{

/**
 * @brief Dense storage with sparse index lookup.
 *
 * Maps sparse indices to densely packed values while maintaining a reverse
 * mapping from dense indices to sparse indices. Deletion uses swap-and-pop,
 * keeping the stored values contiguous.
 *
 * Lookup:
 * - Sparse index lookup: O(1)
 * - Dense index lookup: O(1)
 *
 * Insertion:
 * - O(1) amortized
 *
 * Deletion:
 * - O(1)
 *
 * Iteration:
 * - O(N)
 *
 * Memory:
 * - O(N + S), where N is the number of stored values and S is the highest
 *   sparse index currently allocated.
 *
 * @tparam D Stored data type.
 */
template <typename D> struct SparseSet
{
    //TODO- sparse sets can get really massive if id is like 1 million or stuff.
    // an idea is probably to introduce generations. a way to reuse ids.
    // another idea for this. is a new data structure. the POOL!! / FreeList
    // the pool can determine what is the next entity id to use, the usual map contains the actual entities.
    // since pools whole gimmick is the ability to reuse empty slots in a vector
    //
    //TODO- by using power 2 page size for sparse set pagination, u can prevent doing modulo & division. speeds up sparse sets alot.

    using DenseIndex = unsigned int;
    using SparseIndex = unsigned int;

    /** @brief Represents an invalid index. */
    static constexpr auto InvalidIndex = std::numeric_limits<SparseIndex>::max();

  private:
    /** @brief Contains the actual values. */
    std::vector<D> Dense{};

    /** @brief Maps dense indices to their sparse indices. */
    std::vector<SparseIndex> Indices{};

    /** @brief Maps sparse indices to their dense indices. */
    std::vector<DenseIndex> Sparse{};

  public:
    struct Iterator
    {
        SparseSet* Set;
        DenseIndex Index;

        /** @brief Advances the iterator to the next value. */
        Iterator& operator++()
        {
            ++Index;
            return *this;
        }

        struct DereferencedIterator
        {
            SparseIndex SparseValue;
            D& DenseValue;

            DereferencedIterator* operator->()
            {
                return this;
            }

            const DereferencedIterator* operator->() const
            {
                return this;
            }
        };

        /** @brief Returns the sparse index and corresponding value. */
        DereferencedIterator operator*() const
        {
            return {.SparseValue = Set->Indices[Index], .DenseValue = Set->Dense[Index]};
        }

        DereferencedIterator operator->() const
        {
            return {.SparseValue = Set->Indices[Index], .DenseValue = Set->Dense[Index]};
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

    /** @brief Returns an iterator to the first value. */
    Iterator begin()
    {
        return {this, 0};
    }

    /** @brief Returns an iterator past the last value. */
    Iterator end()
    {
        return {this, Size()};
    }

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

    D& GetUnchecked(const SparseIndex s)
    {
        return Dense[Sparse[s]];
    }

    /** @brief Finds the value associated with the specified sparse index. */
    Iterator Find(const SparseIndex index)
    {
        if (!Contains(index))
            return end();

        return {.Set = this, .Index = Sparse[index]};
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
    template <typename U> requires std::constructible_from<D, U&&>
    D& Push(SparseIndex s, U&& value)
    {
        if (!Contains(s))
        {
            if (s >= Sparse.size())
                Sparse.resize(s + 1, InvalidIndex);

            Sparse[s] = Dense.size();
            Indices.push_back(s);
            Dense.push_back(value);
        }

        return Dense[Sparse[s]];
    }

    /** @brief Constructs a value at the specified sparse index. */
    template <typename... Args> Iterator Emplace(const SparseIndex s, Args&&... args)
    {
        if (!Contains(s))
        {
            if (s >= Sparse.size())
                Sparse.resize(s + 1, InvalidIndex);

            Sparse[s] = Dense.size();
            Indices.push_back(s);
            Dense.emplace_back(std::forward<Args>(args)...);
        }

        return {.Set = this, .Index = Sparse[s]};
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

    /** @brief Removes the value at the specified dense index. */
    void DeleteByIndex(DenseIndex index)
    {
        Delete(Indices[index]);
    }

    /** @brief Removes all values while retaining allocated memory. */
    void Clear()
    {
        Dense.clear();
        Indices.clear();
    }

    /** @brief Reserves dense storage for the specified number of elements. */
    void Reserve(const DenseIndex size)
    {
        Dense.reserve(size);
        Indices.reserve(size);
    }

    /** @brief Returns the number of stored values. */
    DenseIndex Size() const
    {
        return Dense.size();
    }

    /** @brief Returns whether the set contains no values. */
    bool Empty() const
    {
        return Dense.empty();
    }
};

} // namespace N