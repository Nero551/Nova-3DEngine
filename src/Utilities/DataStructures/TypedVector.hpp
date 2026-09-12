#pragma once
#include <vector>
namespace N
{

template <typename T> struct TypedVector
{
    using TypeId = size_t;
    TypeId NextTypeId{};

    std::vector<T> Data{};

    template <typename... Args> bool Contains()
    {
        const TypeId typeId = GetTypeId<Args...>();
        return typeId < Data.size() && Data[typeId] != nullptr;
    }

    template <typename... Args> T& Get()
    {
        if (!Contains<Args...>())
        {
            U::Logger::Fatal("TypedVector does not contain the specified TypeId.");
        }
        return Data[GetTypeId<Args...>()];
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

    template <typename... Args, typename... Parameters> T& Emplace(Parameters&&... parameters)
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
        return Data[typeId];
    }

  private:
    template <typename... Args> TypeId GetTypeId()
    {
        static const TypeId Id = NextTypeId++;
        return Id;
    }
};

} // namespace N