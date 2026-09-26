#pragma once
#include "Event.hpp"

namespace N::C
{

template <typename T> struct Track
{
    Track() {};
    Track(const T& value) : m_Value(value) {}

    const T& Get() const
    {
        return m_Value;
    }

    T& operator()()
    {
        MarkChanged();
        return m_Value;
    }

    const T& operator()() const
    {
        return m_Value;
    }

    void Set(const T& value)
    {
        if (m_Value == value)
        {
            ClearChanged();
            return;
        }
        m_Value = value;
        MarkChanged();
    }

    operator const T&() const
    {
        return m_Value;
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
        return m_Value + value;
    }

    template <typename V> T operator-(const V& value)
    {
        return m_Value - value;
    }

    template <typename V> T operator*(const V& value)
    {
        return m_Value * value;
    }
    template <typename V> T operator/(const V& value)
    {
        return m_Value / value;
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
        return m_Changed;
    }

    void ClearChanged()
    {
        m_Changed = false;
    }

    void MarkChanged()
    {
        if (!m_Changed)
        {
            m_Changed = true;
        }
    }

  private:
    T m_Value{};
    bool m_Changed = false;
};

} // namespace N::C