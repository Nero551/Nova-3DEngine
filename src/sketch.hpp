#pragma once
#include "Core/OuterCore/ECS/Component.hpp"
#include "Utilities/Log.hpp"

#include <concepts>
namespace Sketch
{

template <typename T> struct Traits
{
    static constexpr bool IsComponent = std::derived_from<T, N::Component>;
};

//TODO- store resources in a sparse set using resource ids.
//
//TODO- pools/freelist for generating ids.
//
//TODO- the size of transform component is whats bottlenecking.
// split it. atleast split transform from the matrices (model matrix, normal matrix)

template <typename T> struct FreeList
{
    using Index = unsigned int;

    bool Contains(Index index)
    {
        return index < m_Data.size() && m_Used[index] == true;
    }

    T& Get(Index index)
    {
        if (!Contains(index))
        {
            N::U::Log::Fatal("FreeList Doesn't contain specified index: ", index);
        }
        return m_Data[index];
    }

    void Push(const T& value)
    {
        if (m_Free.empty())
        {
            m_Used.push_back(true);
            m_Data.push_back(value);
        }
        else
        {
            Index index = m_Free.front();
            m_Free.pop();

            m_Used[index] = true;
            m_Data[index] = value;
        }
    }

    void Erase(Index index)
    {
        if (!Contains(index))
        {
            return;
        }

        m_Used[index] = false;
        m_Free.push(index);
    }

    friend std::ostream& operator<<(std::ostream& os, const FreeList& freeList)
    {
        os << "[";

        for (Index i = 0; i < freeList.m_Data.size(); i++)
        {

            if (freeList.m_Used[i] == true)
            {
                os << freeList.m_Data[i];
            }
            else
            {
                os << "Inactive";
            }

            if (i != freeList.m_Data.size() - 1)
            {
                os << ", ";
            }
        }
        os << "]";

        return os;
    }

    Index Size() const
    {
        return m_Data.size();
    }

  private:
    std::vector<T> m_Data;
    std::vector<bool> m_Used;
    std::queue<Index> m_Free;
};

struct Block : N::Component
{
};

struct NotBlock
{
};

inline void Test()
{
    N::U::Log::Info(Traits<Block>::IsComponent);    // true
    N::U::Log::Info(Traits<NotBlock>::IsComponent); // false
}

using Index = unsigned int;

template <typename T, Index Size> struct Array
{
    bool Contains(Index index)
    {
        return index < Size;
    }

    T& At(Index index)
    {
        if (Contains(index))
        {
            return m_Data[index];
        }
        N::U::Log::Fatal("Array: Out of bounds.");
    }

    T& operator[](Index index)
    {
        return m_Data[index];
    }

  private:
    T m_Data[Size];
};

} // namespace Sketch