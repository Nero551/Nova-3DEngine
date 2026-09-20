#pragma once
#include "Utilities/Log.hpp"
namespace N::U
{
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
                os << "Unused";
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

} // namespace N::U