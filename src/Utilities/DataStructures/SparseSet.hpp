#pragma once

namespace N::U
{

/**
 * @brief Sparse-indexed storage with densely packed values.
 *
 * Each sparse index maps to a dense index, allowing O(1) lookup while keeping
 * active entries contiguous. Erasing an entry uses swap-and-pop, so dense
 * indices can change after an erase.
 */
template <typename T> struct SparseSet
{
    using DenseIndex = unsigned int;
    using SparseIndex = unsigned int;

    static constexpr auto InvalidSparseIndex = std::numeric_limits<SparseIndex>::max();

    struct Entry
    {
        SparseIndex SparseIndex;
        T Value;
    };

    template <bool Const> struct BasicIterator
    {
        using SetType = std::conditional_t<Const, const SparseSet, SparseSet>;
        using EntryType = std::conditional_t<Const, const Entry, Entry>;

        SetType* Set;
        DenseIndex Index;

        BasicIterator& operator++()
        {
            ++Index;
            return *this;
        }

        EntryType& operator*() const
        {
            return Set->m_Dense[Index];
        }

        EntryType* operator->() const
        {
            return &Set->m_Dense[Index];
        }

        bool operator==(const BasicIterator& other) const
        {
            return Index == other.Index;
        }

        bool operator!=(const BasicIterator& other) const
        {
            return !(*this == other);
        }
    };

    using Iterator = BasicIterator<false>;
    using ConstIterator = BasicIterator<true>;

    Iterator begin()
    {
        return {.Set = this, .Index = 0};
    }

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

    bool Contains(const SparseIndex index) const
    {
        return index < m_Sparse.size() && m_Sparse[index] != InvalidSparseIndex;
    }

    T& At(const SparseIndex index)
    {
        if (!Contains(index))
            Log::Fatal("SparseSet does not contain the specified sparse index.");

        return m_Dense[m_Sparse[index]].Value;
    }

    const T& At(const SparseIndex index) const
    {
        if (!Contains(index))
            Log::Fatal("SparseSet does not contain the specified sparse index.");

        return m_Dense[m_Sparse[index]].Value;
    }

    T& operator[](const SparseIndex index)
    {
        return m_Dense[m_Sparse[index]].Value;
    }

    const T& operator[](const SparseIndex index) const
    {
        return m_Dense[m_Sparse[index]].Value;
    }

    Entry& AtDense(const DenseIndex index)
    {
        if (index >= m_Dense.size())
            Log::Fatal("SparseSet dense index out of bounds.");

        return m_Dense[index];
    }

    const Entry& AtDense(const DenseIndex index) const
    {
        if (index >= m_Dense.size())
            Log::Fatal("SparseSet dense index out of bounds.");

        return m_Dense[index];
    }

    Iterator Find(const SparseIndex index)
    {
        if (!Contains(index))
            return end();

        return {.Set = this, .Index = m_Sparse[index]};
    }

    ConstIterator Find(const SparseIndex index) const
    {
        if (!Contains(index))
            return end();

        return {.Set = this, .Index = m_Sparse[index]};
    }

    DenseIndex DenseIndexOf(const SparseIndex index) const
    {
        return m_Sparse[index];
    }

    SparseIndex SparseIndexOf(const DenseIndex index) const
    {
        return m_Dense[index].SparseIndex;
    }

    /** @brief Adds a value if the sparse index is unused, otherwise returns the existing value. */
    template <typename U> requires std::constructible_from<T, U&&>
    T& Push(const SparseIndex index, U&& value)
    {
        if (!Contains(index))
        {
            if (index >= m_Sparse.size())
                m_Sparse.resize(index + 1, InvalidSparseIndex);

            m_Sparse[index] = m_Dense.size();
            m_Dense.push_back({.Value = std::forward<U>(value), .SparseIndex = index});
        }

        return m_Dense[m_Sparse[index]].Value;
    }

    /** @brief Constructs a value at the sparse index if it is unused. */
    template <typename... Args> Iterator Emplace(const SparseIndex index, Args&&... args)
    {
        if (!Contains(index))
        {
            if (index >= m_Sparse.size())
                m_Sparse.resize(index + 1, InvalidSparseIndex);

            m_Sparse[index] = m_Dense.size();
            m_Dense.emplace_back(Entry{.Value = T{std::forward<Args>(args)...}, .SparseIndex = index});
        }

        return {.Set = this, .Index = m_Sparse[index]};
    }

    /**
     * @brief Removes an entry while keeping the dense storage packed.
     *
     * The last entry is moved into the erased entry's position, so dense
     * indices are not stable across erases.
     */
    bool Erase(const SparseIndex index)
    {
        if (!Contains(index))
            return false;

        const DenseIndex denseIndex = m_Sparse[index];

        if (denseIndex != m_Dense.size() - 1)
        {
            Entry& lastEntry = m_Dense.back();

            std::swap(m_Dense[denseIndex], lastEntry);

            m_Sparse[lastEntry.SparseIndex] = denseIndex;
        }

        m_Dense.pop_back();
        m_Sparse[index] = InvalidSparseIndex;

        return true;
    }

    bool EraseByDense(const DenseIndex index)
    {
        return Erase(m_Dense[index].SparseIndex);
    }

    void Clear()
    {
        m_Dense.clear();

        for (DenseIndex& index : m_Sparse)
            index = InvalidSparseIndex;
    }

    void Reserve(const DenseIndex size)
    {
        m_Dense.reserve(size);
        m_Sparse.reserve(size);
    }

    DenseIndex Size() const
    {
        return m_Dense.size();
    }

    bool Empty() const
    {
        return m_Dense.empty();
    }

  private:
    std::vector<Entry> m_Dense{};
    std::vector<DenseIndex> m_Sparse{};
};

} // namespace N::U