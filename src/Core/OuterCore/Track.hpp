#pragma once
#include "Event.hpp"

namespace N
{

template <typename T> struct Track
{
    Track() {};
    Track(const T& value) : Value(value) {}

    const T& Get() const
    {
        return Value;
    }

    T& operator()()
    {
        MarkChanged();
        return Value;
    }

    const T& operator()() const
    {
        return Value;
    }

    void Set(const T& value)
    {
        if (Value == value)
        {
            ClearChanged();
            return;
        }
        Value = value;
        MarkChanged();
    }

    operator const T&() const
    {
        return Value;
    }

    template <typename V> Track& operator+=(const V& value)
    {
        operator()() += value;
        return *this;
    }

    template <typename V> Track& operator-=(const V& value)
    {
        operator()() -= value;
        return *this;
    }

    template <typename V> Track& operator*=(const V& value)
    {
        operator()() *= value;
        return *this;
    }

    template <typename V> Track& operator/=(const V& value)
    {
        operator()() /= value;
        return *this;
    }

    template <typename V> T operator+(const V& value)
    {
        return Value + value;
    }

    template <typename V> T operator-(const V& value)
    {
        return Value - value;
    }

    template <typename V> T operator*(const V& value)
    {
        return Value * value;
    }
    template <typename V> T operator/(const V& value)
    {
        return Value / value;
    }

    template <typename V>
    T friend operator+(const V& other, const Track& value) requires(!std::same_as<V, Track>)
    {
        return other + value;
    }
    template <typename V>
    T friend operator-(const V& other, const Track& value) requires(!std::same_as<V, Track>)
    {
        return other - value;
    }

    template <typename V>
    T friend operator*(const V& other, const Track& value) requires(!std::same_as<V, Track>)
    {
        return other * value;
    }

    template <typename V>
    T friend operator/(const V& other, const Track& value) requires(!std::same_as<V, Track>)
    {
        return other / value;
    }

    Track& operator=(const T& value)
    {
        Set(value);
        return *this;
    }

    bool IsChanged() const
    {
        return Changed;
    }

    void ClearChanged()
    {
        Changed = false;
    }

    void MarkChanged()
    {
        if (!Changed)
        {
            Changed = true;
        }
    }

  private:
    T Value{};
    bool Changed = false;
};

} // namespace N