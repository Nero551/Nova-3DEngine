#pragma once
#include "Event.hpp"

namespace N
{

struct TrackChanged : Event<TrackChanged>
{
    TrackChanged() {}
};

//TODO- events dont work
template <typename T> struct Track
{
    Track() {};
    Track(const T& value) : Value(value) {}
    template <typename F> Track(const T& value, F&& callback) : Value(value)
    {
        Connect(std::forward<F>(callback));
    }

    template <typename F> size_t Connect(F&& callback)
    {
        const auto id = Event.Sub([callback = std::forward<F>(callback)](TrackChanged&) { callback(); });
        return id;
    }
    void Disconnect(size_t subscription)
    {
        Event.Unsub(subscription);
    }

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
        Value = value;
        MarkChanged();
    }

    operator const T&() const
    {
        return Value;
    }

    operator T&()
    {
        MarkChanged();
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
        if (Changed)
        {
            return;
        }
        Changed = true;
        Event.Fire();
    }

  private:
    T Value{};
    bool Changed = false;
    TrackChanged Event{};
};

} // namespace N