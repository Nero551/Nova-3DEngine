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
// i actually dont need to make stuff like Material and FrameBuffer start storing resource ids if i do that.
// since resources are stored as unique pointers anyway to avoid type erasure.
// sparse set moving the unique pointers around won't invalidate other pointers to the resources.
//
//TODO- pools/freelist for generating ids.
//
//TODO- the size of transform component is whats bottlenecking.
// split it. atleast split transform from the matrices (model matrix, normal matrix)

//TODO- add operator<< to all custom data structures. clean up data structure code.

//TODO- add operator[] for data structures instead of GetUnchecked.

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