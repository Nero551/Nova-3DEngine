#pragma once
#include "Utilities/Logger.hpp"
namespace N
{

template <typename T> struct TypedVector
{
    using TypeId = unsigned int;
    struct Iterator
    {
        TypedVector* TypedVector;
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
            return TypedVector->Data[Index];
        }

        T* operator->() const
        {
            return &TypedVector->Data[Index];
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

        /**
         * @brief Advances the iterator to the next value.
         *
         * @return Reference to this iterator.
         */
        ConstIterator& operator++()
        {
            ++Index;
            return *this;
        }

        /**
         * @brief Returns the current value.
         *
         * @return Reference to the current value.
         */
        const T& operator*() const
        {
            return TypedVector->Data[Index];
        }

        const T* operator->() const
        {
            return &TypedVector->Data[Index];
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
        return {.TypedVector = this, .Index = Data.size()};
    }

    ConstIterator begin() const
    {
        return {.TypedVector = this, .Index = 0};
    }

    ConstIterator end() const
    {
        return {.TypedVector = this, .Index = Data.size()};
    }

    template <typename... Args> bool Contains()
    {
        const TypeId typeId = GetTypeId<Args...>();
        return typeId < Data.size();
    }

    template <typename... Args> T& Get()
    {
        if (!Contains<Args...>())
        {
            U::Logger::Fatal("TypedVector does not contain the specified TypeId.");
        }
        return Data[GetTypeId<Args...>()];
    }

    template <typename... Args> T& GetUnchecked()
    {
        return Data[GetTypeId<Args...>()];
    }

    template <typename... Args> Iterator Find()
    {
        TypeId typeId = GetTypeId<Args...>();

        if (typeId >= Data.size())
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
            if (Data.size() <= typeId)
            {
                Data.resize(typeId + 1);
            }
            Data[typeId] = value;
        }

        return Data[typeId];
    }

    void Clear()
    {
        Data.clear();
    }

    template <typename... Args, typename... Parameters> Iterator Emplace(Parameters&&... parameters)
    {
        const TypeId typeId = GetTypeId<Args...>();

        if (!Contains<Args...>())
        {

            if (Data.size() <= typeId)
            {
                Data.resize(typeId + 1);
            }

            Data[typeId] = T(std::forward<Parameters>(parameters)...);
        }
        return {this, typeId};
    }

    TypeId Size() const
    {
        return Data.size();
    }

  private:
    TypeId NextTypeId{};
    std::vector<T> Data{};

    template <typename... Args> TypeId GetTypeId()
    {
        static const TypeId Id = NextTypeId++;
        return Id;
    }
};

} // namespace N