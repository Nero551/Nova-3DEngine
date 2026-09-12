#pragma once

#include <vector>

namespace N
{

/**
 * @brief Stores values densely while providing sparse-index lookup.
 *
 * Maintains a densely packed array of values and a sparse array mapping
 * sparse indices to dense indices. Each dense entry also stores its sparse
 * index, allowing constant-time reverse lookup and swap-and-pop deletion.
 *
 * @tparam D Value type stored by the set.
 *
 * @par Complexity
 * - Sparse lookup: O(1)
 * - Dense lookup: O(1)
 * - Insertion: O(1) amortized
 * - Deletion: O(1)
 * - Iteration: O(N)
 *
 * @par Memory
 * Uses O(N + S) memory, where N is the number of stored values and S is
 * the highest sparse index allocated.
 */
template <typename D> struct SparseSet
{
    using DenseIndex = unsigned int;
    using SparseIndex = unsigned int;

    /** @brief Represents an invalid dense index. */
    static constexpr auto InvalidIndex = std::numeric_limits<SparseIndex>::max();

    /**
     * @brief Stores a value together with its sparse index.
     *
     * Keeping the sparse index with the value allows dense iteration to
     * retrieve both without accessing the sparse array.
     */
    struct Entry
    {
        D Value;
        SparseIndex Index;
    };

  private:
    /** @brief Densely packed values. */
    std::vector<Entry> Dense{};

    /** @brief Maps sparse indices to dense indices. */
    std::vector<DenseIndex> Sparse{};

  public:
    /**
     * @brief Iterator over densely stored entries.
     *
     * Dereferencing provides both the sparse index and the stored value.
     */
    struct Iterator
    {
        SparseSet* Set;
        DenseIndex Index;

        /** @brief Advances the iterator to the next entry. */
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
            return {.SparseValue = Set->Dense[Index].Index, .DenseValue = Set->Dense[Index].Value};
        }

        DereferencedIterator operator->() const
        {
            return {.SparseValue = Set->Dense[Index].Index, .DenseValue = Set->Dense[Index].Value};
        }

        /** @brief Compares two iterators for inequality. */
        bool operator!=(const Iterator& other) const
        {
            return Index != other.Index;
        }

        /** @brief Compares two iterators for equality. */
        bool operator==(const Iterator& other) const
        {
            return Index == other.Index;
        }
    };

    /** @brief Returns an iterator to the first entry. */
    Iterator begin()
    {
        return {this, 0};
    }

    /** @brief Returns an iterator past the last entry. */
    Iterator end()
    {
        return {this, Size()};
    }

    /** @brief Returns whether the specified sparse index exists. */
    bool Contains(const SparseIndex s) const
    {
        return s < Sparse.size() && Sparse[s] != InvalidIndex;
    }

    /**
     * @brief Returns the value associated with a sparse index.
     *
     * @param s Sparse index to look up.
     * @return Reference to the associated value.
     *
     * @throws Fatal error if the sparse index does not exist.
     */
    D& Get(const SparseIndex s)
    {
        if (!Contains(s))
            U::Logger::Fatal("SparseSet does not contain the specified sparse index.");

        return Dense[Sparse[s]].Value;
    }

    /**
     * @brief Returns the value associated with a sparse index without checking.
     *
     * @param s Sparse index to look up.
     * @return Reference to the associated value.
     *
     * @warning The sparse index must exist.
     */
    D& GetUnchecked(const SparseIndex s)
    {
        return Dense[Sparse[s]].Value;
    }

    /**
     * @brief Finds an entry by sparse index.
     *
     * @param s Sparse index to find.
     * @return Iterator to the entry, or end() if it does not exist.
     */
    Iterator Find(const SparseIndex s)
    {
        if (!Contains(s))
            return end();

        return {.Set = this, .Index = Sparse[s]};
    }

    /**
     * @brief Returns the value at a dense index.
     *
     * @param index Dense index to access.
     * @return Reference to the stored value.
     *
     * @throws Fatal error if the dense index is out of bounds.
     */
    D& GetByIndex(const DenseIndex index)
    {
        if (index >= Dense.size())
            U::Logger::Fatal("SparseSet dense index out of bounds.");

        return Dense[index].Value;
    }

    /**
     * @brief Returns the sparse index associated with a dense index.
     *
     * @param index Dense index to access.
     * @return The corresponding sparse index.
     */
    SparseIndex GetSparseIndex(const DenseIndex index) const
    {
        return Dense[index].Index;
    }

    /**
     * @brief Returns the complete entry associated with a sparse index.
     *
     * @param s Sparse index to access.
     * @return Reference to the corresponding entry.
     *
     * @throws Fatal error if the sparse index does not exist.
     */
    Entry& GetEntry(const SparseIndex s)
    {
        if (!Contains(s))
            U::Logger::Fatal("SparseSet does not contain the specified sparse index.");

        return Dense[Sparse[s]];
    }

    /**
     * @brief Returns the complete entry without checking.
     *
     * @param s Sparse index to access.
     * @return Reference to the corresponding entry.
     *
     * @warning The sparse index must exist.
     */
    Entry& GetEntryUnchecked(const SparseIndex s)
    {
        return Dense[Sparse[s]];
    }

    /**
     * @brief Returns the dense index associated with a sparse index.
     *
     * @param s Sparse index to look up.
     * @return The corresponding dense index.
     *
     * @warning The sparse index must exist.
     */
    DenseIndex GetDenseIndex(const SparseIndex s) const
    {
        return Sparse[s];
    }

    /**
     * @brief Inserts a value at a sparse index.
     *
     * If the sparse index already exists, its existing value is returned
     * and no insertion occurs.
     *
     * @param s Sparse index to associate with the value.
     * @param value Value to insert.
     * @return Reference to the stored value.
     */
    template <typename U> requires std::constructible_from<D, U&&>
    D& Push(const SparseIndex s, U&& value)
    {
        if (!Contains(s))
        {
            if (s >= Sparse.size())
                Sparse.resize(s + 1, InvalidIndex);

            Sparse[s] = Dense.size();
            Dense.push_back({std::forward<U>(value), s});
        }

        return Dense[Sparse[s]].Value;
    }

    /**
     * @brief Constructs a value at a sparse index.
     *
     * If the sparse index already exists, its existing entry is returned.
     *
     * @param s Sparse index to associate with the value.
     * @param args Arguments forwarded to the value constructor.
     * @return Iterator to the stored entry.
     */
    template <typename... Args> Iterator Emplace(const SparseIndex s, Args&&... args)
    {
        if (!Contains(s))
        {
            if (s >= Sparse.size())
                Sparse.resize(s + 1, InvalidIndex);

            Sparse[s] = Dense.size();

            Dense.emplace_back(Entry{.Value = D{std::forward<Args>(args)...}, .Index = s});
        }

        return {.Set = this, .Index = Sparse[s]};
    }

    /**
     * @brief Removes the value associated with a sparse index.
     *
     * Uses swap-and-pop to maintain dense storage.
     *
     * @param s Sparse index to remove.
     */
    void Delete(const SparseIndex s)
    {
        if (!Contains(s))
            return;

        const DenseIndex index = Sparse[s];

        if (index != Dense.size() - 1)
        {
            Entry& lastDenseEntry = Dense.back();

            std::swap(Dense[index], lastDenseEntry);

            Sparse[lastDenseEntry.Index] = index;
        }

        Dense.pop_back();
        Sparse[s] = InvalidIndex;
    }

    /**
     * @brief Removes the value at a dense index.
     *
     * @param index Dense index to remove.
     */
    void DeleteByIndex(const DenseIndex index)
    {
        Delete(Dense[index].Index);
    }

    /** @brief Removes all values while retaining allocated storage. */
    void Clear()
    {
        Dense.clear();

        for (DenseIndex& index : Sparse)
            index = InvalidIndex;
    }

    /**
     * @brief Reserves dense storage for a number of entries.
     *
     * @param size Number of entries to reserve.
     */
    void Reserve(const DenseIndex size)
    {
        Dense.reserve(size);
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
