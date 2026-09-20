#pragma once

#include "Utilities/Log.hpp"

namespace N::U
{

template <typename T> struct TypedVector
{
    using TypeId = unsigned int;

    struct Iterator
    {
        TypedVector* TypedVector;
        size_t Index;

        Iterator& operator++()
        {
            ++Index;
            return *this;
        }

        T& operator*() const
        {
            return TypedVector->m_Data[Index];
        }

        T* operator->() const
        {
            return &TypedVector->m_Data[Index];
        }

        bool operator!=(const Iterator& other) const
        {
            return Index != other.Index;
        }

        bool operator==(const Iterator& other) const
        {
            return Index == other.Index;
        }
    };

    struct ConstIterator
    {
        const TypedVector* TypedVector;
        size_t Index;

        ConstIterator& operator++()
        {
            ++Index;
            return *this;
        }

        const T& operator*() const
        {
            return TypedVector->m_Data[Index];
        }

        const T* operator->() const
        {
            return &TypedVector->m_Data[Index];
        }

        bool operator!=(const ConstIterator& other) const
        {
            return Index != other.Index;
        }

        bool operator==(const ConstIterator& other) const
        {
            return Index == other.Index;
        }
    };

    Iterator begin()
    {
        return {.TypedVector = this, .Index = 0};
    }

    Iterator end()
    {
        return {.TypedVector = this, .Index = m_Data.size()};
    }

    ConstIterator begin() const
    {
        return {.TypedVector = this, .Index = 0};
    }

    ConstIterator end() const
    {
        return {.TypedVector = this, .Index = m_Data.size()};
    }

    template <typename... Args> bool Contains()
    {
        const TypeId typeId = GetTypeId<Args...>();
        return typeId < m_Data.size();
    }

    template <typename... Args> T& Get()
    {
        if (!Contains<Args...>())
        {
            U::Log::Fatal("TypedVector does not contain the specified TypeId.");
        }

        return m_Data[GetTypeId<Args...>()];
    }

    template <typename... Args> T& GetUnchecked()
    {
        return m_Data[GetTypeId<Args...>()];
    }

    template <typename... Args> Iterator Find()
    {
        TypeId typeId = GetTypeId<Args...>();

        if (typeId >= m_Data.size())
        {
            return end();
        }

        return {.TypedVector = this, .Index = typeId};
    }

    template <typename... Args> T& Push(T& value)
    {
        TypeId typeId = GetTypeId<Args...>();

        if (!Contains<Args...>())
        {
            if (m_Data.size() <= typeId)
            {
                m_Data.resize(typeId + 1);
            }

            m_Data[typeId] = value;
        }

        return m_Data[typeId];
    }

    void Clear()
    {
        m_Data.clear();
    }

    template <typename... Args, typename... Parameters> Iterator Emplace(Parameters&&... parameters)
    {
        const TypeId typeId = GetTypeId<Args...>();

        if (!Contains<Args...>())
        {
            if (m_Data.size() <= typeId)
            {
                m_Data.resize(typeId + 1);
            }

            m_Data[typeId] = T(std::forward<Parameters>(parameters)...);
        }

        return {this, typeId};
    }

    TypeId Size() const
    {
        return m_Data.size();
    }

  private:
    TypeId m_NextTypeId{};
    std::vector<T> m_Data{};

    template <typename... Args> TypeId GetTypeId()
    {
        static const TypeId Id = m_NextTypeId++;
        return Id;
    }
};

} // namespace N::U