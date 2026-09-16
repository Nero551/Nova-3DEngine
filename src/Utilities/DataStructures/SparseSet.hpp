#pragma once
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
    //TODO- the design where i merge indices and dense into 1 vector is called AoS.
    // the one where they are separate is called SoA.
    // AoS is good if am accessing the indices that much while iterating.
    // SoA is better if am not, improves cache locality.
    // depends on the findings, but i should probably use SoA.

    /** @brief Densely packed values. */
    std::vector<Entry> m_Dense{};

    /** @brief Maps sparse indices to dense indices. */
    std::vector<DenseIndex> m_Sparse{};

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
        DereferencedIterator operator*()
        {
            auto& entry = Set->m_Dense[Index];
            return {.SparseValue = entry.Index, .DenseValue = entry.Value};
        }

        DereferencedIterator operator->()
        {
            auto& entry = Set->m_Dense[Index];
            return {.SparseValue = entry.Index, .DenseValue = entry.Value};
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

    struct ConstIterator
    {
        const SparseSet* Set;
        DenseIndex Index;

        /** @brief Advances the iterator to the next entry. */
        ConstIterator& operator++()
        {
            ++Index;
            return *this;
        }

        struct DereferencedConstIterator
        {
            SparseIndex SparseValue;
            const D& DenseValue;

            const DereferencedConstIterator* operator->() const
            {
                return this;
            }
        };

        /** @brief Returns the sparse index and corresponding value. */
        DereferencedConstIterator operator*() const
        {
            auto& entry = Set->m_Dense[Index];
            return {.SparseValue = entry.Index, .DenseValue = entry.Value};
        }

        DereferencedConstIterator operator->() const
        {
            auto& entry = Set->m_Dense[Index];
            return {.SparseValue = entry.Index, .DenseValue = entry.Value};
        }

        /** @brief Compares two iterators for inequality. */
        bool operator!=(const ConstIterator& other) const
        {
            return Index != other.Index;
        }

        /** @brief Compares two iterators for equality. */
        bool operator==(const ConstIterator& other) const
        {
            return Index == other.Index;
        }
    };

    /** @brief Returns an iterator to the first entry. */
    Iterator begin()
    {
        return {.Set = this, .Index = 0};
    }

    /** @brief Returns an iterator past the last entry. */
    Iterator end()
    {
        return {.Set = this, .Index = Size()};
    }

    ConstIterator begin() const
    {
        return {.Set = this, .Index = 0};
    }

    ConstIterator end() const
    {
        return {.Set = this, .Index = Size()};
    }

    /** @brief Returns whether the specified sparse index exists. */
    bool Contains(const SparseIndex s) const
    {
        return s < m_Sparse.size() && m_Sparse[s] != InvalidIndex;
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
            U::Log::Fatal("SparseSet does not contain the specified sparse index.");

        return m_Dense[m_Sparse[s]].Value;
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
        return m_Dense[m_Sparse[s]].Value;
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

        return {.Set = this, .Index = m_Sparse[s]};
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
        if (index >= m_Dense.size())
            U::Log::Fatal("SparseSet dense index out of bounds.");

        return m_Dense[index].Value;
    }

    D& GetByIndexUnchecked(const DenseIndex index)
    {
        return m_Dense[index].Value;
    }

    /**
     * @brief Returns the sparse index associated with a dense index.
     *
     * @param index Dense index to access.
     * @return The corresponding sparse index.
     */
    SparseIndex GetSparseIndex(const DenseIndex index) const
    {
        return m_Dense[index].Index;
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
            U::Log::Fatal("SparseSet does not contain the specified sparse index.");

        return m_Dense[m_Sparse[s]];
    }

    Entry& GetEntryByIndexUnchecked(const DenseIndex index)
    {
        return m_Dense[index];
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
        return m_Dense[m_Sparse[s]];
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
        return m_Sparse[s];
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
            if (s >= m_Sparse.size())
                m_Sparse.resize(s + 1, InvalidIndex);

            m_Sparse[s] = m_Dense.size();
            m_Dense.push_back({std::forward<U>(value), s});
        }

        return m_Dense[m_Sparse[s]].Value;
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
            if (s >= m_Sparse.size())
                m_Sparse.resize(s + 1, InvalidIndex);

            m_Sparse[s] = m_Dense.size();

            m_Dense.emplace_back(Entry{.Value = D{std::forward<Args>(args)...}, .Index = s});
        }

        return {.Set = this, .Index = m_Sparse[s]};
    }

    /**
     * @brief Removes the value associated with a sparse index.
     *
     * Uses swap-and-pop to maintain dense storage.
     *
     * @param s Sparse index to remove.
     * @return if value was there and got deleted.
     */
    bool Erase(const SparseIndex s)
    {
        if (!Contains(s))
        {
            return false;
        }

        const DenseIndex index = m_Sparse[s];

        if (index != m_Dense.size() - 1)
        {
            Entry& lastDenseEntry = m_Dense.back();

            std::swap(m_Dense[index], lastDenseEntry);

            m_Sparse[lastDenseEntry.Index] = index;
        }

        m_Dense.pop_back();
        m_Sparse[s] = InvalidIndex;

        return true;
    }

    /**
     * @brief Removes the value at a dense index.
     *
     * @param index Dense index to remove.
     */
    void DeleteByIndex(const DenseIndex index)
    {
        Delete(m_Dense[index].Index);
    }

    /** @brief Removes all values while retaining allocated storage. */
    void Clear()
    {
        m_Dense.clear();

        for (DenseIndex& index : m_Sparse)
            index = InvalidIndex;
    }

    /**
     * @brief Reserves dense storage for a number of entries.
     *
     * @param size Number of entries to reserve.
     */
    void Reserve(const DenseIndex size)
    {
        m_Dense.reserve(size);
        m_Sparse.reserve(size);
    }

    /** @brief Returns the number of stored values. */
    DenseIndex Size() const
    {
        return m_Dense.size();
    }

    /** @brief Returns whether the set contains no values. */
    bool Empty() const
    {
        return m_Dense.empty();
    }
};

} // namespace N
