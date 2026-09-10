#pragma once

#include <string_view>

namespace N::Reflection
{

using TypeId = unsigned long;

template <typename T> consteval std::string_view GetTypeName()
{
    constexpr std::string_view Function = __PRETTY_FUNCTION__;
    constexpr std::string_view Prefix = "T = ";
    constexpr std::size_t Start = Function.find(Prefix) + Prefix.size();
    constexpr std::size_t End = Function.find(']', Start);

    return Function.substr(Start, End - Start);
}

consteval unsigned long Hash(const std::string_view typeName)
{
    unsigned long hashValue = 0xcbf29ce484222325ULL;
    for (char c : typeName)
    {
        constexpr unsigned long prime = 0x100000001b3ULL;
        hashValue ^= static_cast<unsigned long>(c);
        hashValue *= prime;
    }
    return hashValue;
}

template <typename T> consteval TypeId GetTypeId()
{
    return Hash(GetTypeName<T>());
}

struct TypeInfo final
{
    std::string_view Name{};
    unsigned int Size = 0;
    unsigned int Align = 0;
    TypeId Id = 0;

    friend std::ostream& operator<<(std::ostream& os, const TypeInfo& typeInfo)
    {
        os << "TypeInfo {\n"
           << "    Name:  " << typeInfo.Name << '\n'
           << "    Size:  " << typeInfo.Size << '\n'
           << "    Align: " << typeInfo.Align << '\n'
           << "    Id:    " << std::hex << typeInfo.Id << std::dec << '\n'
           << '}';

        return os;
    }
};

template <typename T> consteval TypeInfo GetTypeInfo()
{
    return {.Name = GetTypeName<T>(), .Size = sizeof(T), .Align = alignof(T), .Id = GetTypeId<T>()};
}

struct AttributeInfo final
{
    std::string_view Name{};
    unsigned int Offset = 0;
    TypeInfo Type;

    friend std::ostream& operator<<(std::ostream& Os, const AttributeInfo& AttributeInfo)
    {
        Os << "AttributeInfo {\n"
           << "    Name:   " << AttributeInfo.Name << '\n'
           << "    Offset: " << AttributeInfo.Offset << '\n'
           << "    Type:   " << AttributeInfo.Type << '\n'
           << '}';

        return Os;
    }
};

#define REGISTER_ATTRIBUTE(Type, Member)                                                           \
    N::Reflection::RegisterAttribute<decltype(Type::Member)>(#Member, offsetof(Type, Member))

template <typename A>
consteval AttributeInfo RegisterAttribute(const std::string_view name, unsigned int offset)
{
    return {.Name = name, .Offset = offset, .Type = GetTypeInfo<A>()};
}

} // namespace N::Reflection