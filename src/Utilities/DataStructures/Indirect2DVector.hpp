#pragma once
#include "Utilities/Logger.hpp"

#include <limits>
#include <utility>
#include <vector>

namespace N
{

/**
 * @brief Dense storage with two-dimensional sparse lookup.
 *
 * Maps a pair of indices `(a, b)` to densely packed values while maintaining
 * a reverse mapping from dense indices to their two-dimensional keys.
 * Deletion uses swap-and-pop, keeping the stored values contiguous.
 *
 * Lookup:
 * - Two-dimensional lookup `(a, b)`: O(1)
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
 * - O(N + A * B), where N is the number of stored values and A/B represent
 *   the allocated dimensions of the two-dimensional lookup.
 *
 * @tparam T The type of values stored by the container.
 */
template <typename T> struct Indirect2DVector
{
    using Index = size_t;

    /** @brief Represents an invalid dense index. */
    static constexpr Index InvalidIndex = std::numeric_limits<Index>::max();

    /**
     * @brief Identifies a value using its two-dimensional lookup indices.
     */
    struct Key
    {
        Index A;
        Index B;
    };

    /**
     * @brief Two-dimensional lookup table mapping `(a, b)` to a dense data index.
     */
    std::vector<std::vector<Index>> Lookup{};

    /**
     * @brief Dense storage containing all inserted values.
     *
     * Values remain contiguous. Deletion uses swap-and-pop, so element order
     * and dense indices are not stable.
     */
    std::vector<T> Data{};

    /**
     * @brief Maps dense indices to their corresponding two-dimensional keys.
     *
     * Used to update the lookup when an element is moved during deletion.
     */
    std::vector<Key> Indices{};

    /**
     * @brief Inserts a value at the specified indices if it does not exist.
     *
     * If `(a, b)` already exists, the existing value is returned and the
     * provided value is ignored.
     *
     * @param a First lookup index.
     * @param b Second lookup index.
     * @param value Value to insert.
     * @return Reference to the existing or newly inserted value.
     */
    T& Push(Index a, Index b, const T& value)
    {
        Index& index = ResizeLookup(a, b)[b];

        if (index == InvalidIndex)
        {
            index = static_cast<Index>(Data.size());
            Data.push_back(value);
            Indices.push_back({a, b});
        }

        return Data[index];
    }

    /**
     * @brief Checks whether a value exists at the specified indices.
     *
     * @param a First lookup index.
     * @param b Second lookup index.
     * @return True if a value exists at `(a, b)`, otherwise false.
     */
    bool Contains(const Index a, const Index b) const
    {
        if (a >= Lookup.size())
        {
            return false;
        }

        const auto& row = Lookup[a];

        return b < row.size() && row[b] != InvalidIndex;
    }

    /**
     * @brief Retrieves the value at the specified indices.
     *
     * @param a First lookup index.
     * @param b Second lookup index.
     * @return Reference to the stored value.
     *
     * @note Logs a fatal error if the value does not exist.
     */
    T& Get(Index a, Index b)
    {
        if (!Contains(a, b))
        {
            U::Logger::Fatal("Indirect2DVector: Index does not exist.");
        }

        return Data[Lookup[a][b]];
    }

    /**
     * @brief Retrieves the value at the specified indices.
     *
     * @param a First lookup index.
     * @param b Second lookup index.
     * @return Const reference to the stored value.
     *
     * @note Logs a fatal error if the value does not exist.
     */
    const T& Get(Index a, Index b) const
    {
        if (!Contains(a, b))
        {
            U::Logger::Fatal("Indirect2DVector: Index does not exist.");
        }

        return Data[Lookup[a][b]];
    }

    /**
     * @brief Constructs a value at the specified indices if it does not exist.
     *
     * If `(a, b)` already exists, the existing value is returned and no
     * construction takes place.
     *
     * @tparam Args Constructor argument types.
     * @param a First lookup index.
     * @param b Second lookup index.
     * @param args Arguments forwarded to the value's constructor.
     * @return Reference to the existing or newly constructed value.
     */
    template <typename... Args> T& Emplace(Index a, Index b, Args&&... args)
    {
        Index& index = ResizeLookup(a, b)[b];

        if (index == InvalidIndex)
        {
            index = static_cast<Index>(Data.size());
            Data.emplace_back(std::forward<Args>(args)...);
            Indices.push_back({a, b});
        }

        return Data[index];
    }

    /**
     * @brief Removes the value at the specified indices.
     *
     * Uses swap-and-pop to keep the dense storage contiguous. If the removed
     * value is not the last element, the last value is moved into its position
     * and its lookup entry is updated.
     *
     * @param a First lookup index.
     * @param b Second lookup index.
     */
    void Delete(Index a, Index b)
    {
        if (!Contains(a, b))
        {
            return;
        }

        const Index index = Lookup[a][b];
        const Index lastIndex = Data.size() - 1;

        if (index != lastIndex)
        {
            const Key lastKey = Indices.back();

            std::swap(Data[index], Data.back());
            std::swap(Indices[index], Indices.back());

            Lookup[lastKey.A][lastKey.B] = index;
        }

        Data.pop_back();
        Indices.pop_back();
        Lookup[a][b] = InvalidIndex;
    }

    /**
     * @brief Reserves storage for the specified number of values.
     *
     * Reserves storage for both dense values and their corresponding keys.
     *
     * @param count Number of values to reserve.
     */
    void Reserve(Index count)
    {
        Data.reserve(count);
        Indices.reserve(count);
    }

    /**
     * @brief Iterator over the densely stored values.
     *
     * Iteration follows the order of the dense `Data` storage. Deletion may
     * change the order of values due to swap-and-pop.
     */
    struct Iterator
    {
        Indirect2DVector& IndirectVector;
        size_t Index;

        /**
         * @brief Advances the iterator to the next value.
         *
         * @return Reference to this iterator.
         */
        Iterator& operator++()
        {
            ++Index;
            return *this;
        }

        /**
         * @brief Returns the current value.
         *
         * @return Reference to the current value.
         */
        T& operator*() const
        {
            return IndirectVector.Data[Index];
        }

        /**
         * @brief Compares two iterators for inequality.
         *
         * @param other Iterator to compare against.
         * @return True if the iterators refer to different positions.
         */
        bool operator!=(const Iterator& other) const
        {
            return Index != other.Index;
        }

        /**
         * @brief Compares two iterators for equality.
         *
         * @param other Iterator to compare against.
         * @return True if the iterators refer to the same position.
         */
        bool operator==(const Iterator& other) const
        {
            return Index == other.Index;
        }
    };

    /**
     * @brief Returns an iterator to the first stored value.
     *
     * @return Iterator to the first value.
     */
    Iterator begin()
    {
        return {*this, 0};
    }

    /**
     * @brief Returns an iterator past the last stored value.
     *
     * @return Iterator past the final value.
     */
    Iterator end()
    {
        return {*this, Data.size()};
    }

  private:
    /**
     * @brief Ensures that the lookup contains the specified indices.
     *
     * Expands both the outer lookup and the requested row when necessary.
     * Newly created entries are initialized to InvalidIndex.
     *
     * @param a First lookup index.
     * @param b Second lookup index.
     * @return Reference to the lookup row for `a`.
     */
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