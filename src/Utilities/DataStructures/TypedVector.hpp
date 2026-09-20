#pragma once

#include "Utilities/Log.hpp"

namespace N::U
{

/**
 * @brief Vector storage indexed by a unique ID for each template type combination.
 *
 * Each TypedVector instance assigns TypeIds sequentially as new type combinations
 * are requested. The TypeId is then used directly as the index into the dense
 * storage.
 */
template <typename T> struct TypedVector
{
    using TypeId = unsigned int;

    template <bool Const> struct BasicIterator
    {
        using TypedVectorType = std::conditional_t<Const, const TypedVector, TypedVector>;
        using ReturnType = std::conditional_t<Const, const T, T>;

        TypedVectorType* TypedVector;
        TypeId Index;

        BasicIterator& operator++()
        {
            ++Index;
            return *this;
        }

        ReturnType& operator*() const
        {
            return TypedVector->m_Data[Index];
        }

        ReturnType* operator->() const
        {
            return &TypedVector->m_Data[Index];
        }

        bool operator!=(const BasicIterator& other) const
        {
            return Index != other.Index;
        }

        bool operator==(const BasicIterator& other) const
        {
            return Index == other.Index;
        }
    };

    using Iterator = BasicIterator<false>;
    using ConstIterator = BasicIterator<true>;

    Iterator begin()
    {
        return {.TypedVector = this, .Index = 0};
    }

    Iterator end()
    {
        return {.TypedVector = this, .Index = Size()};
    }

    ConstIterator begin() const
    {
        return {.TypedVector = this, .Index = 0};
    }

    ConstIterator end() const
    {
        return {.TypedVector = this, .Index = Size()};
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
        const TypeId typeId = GetTypeId<Args...>();

        if (typeId >= m_Data.size())
        {
            return end();
        }

        return {.TypedVector = this, .Index = typeId};
    }

    /** @brief Stores a value for the specified type combination if it does not exist. */
    template <typename... Args> T& Push(T& value)
    {
        const TypeId typeId = GetTypeId<Args...>();

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

    /** @brief Constructs a value for the specified type combination if it does not exist. */
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
    std::vector<TypeId> m_TypeIds{};
    static constexpr TypeId InvalidTypeId = std::numeric_limits<TypeId>::max();

    template <typename... Args> TypeId GetTypeId()
    {
        const TypeId globalId = GetGlobalTypeId<Args...>();

        if (m_TypeIds.size() <= globalId)
        {
            m_TypeIds.resize(globalId + 1, InvalidTypeId);
        }

        TypeId& localId = m_TypeIds[globalId];

        if (localId == InvalidTypeId)
        {
            localId = m_NextTypeId++;
        }

        return localId;
    }

    template <typename... Args> static TypeId GetGlobalTypeId()
    {
        static const TypeId Id = GetNextGlobalTypeId();
        return Id;
    }

    static TypeId GetNextGlobalTypeId()
    {
        static TypeId Id{};
        return Id++;
    }
};

} // namespace N::U