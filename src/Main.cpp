#include "Core/InnerCore/Engine.hpp"

static void Testing() {}

#pragma once
#include <tracy/Tracy.hpp>

#include <cstdio>

void* operator new(std::size_t count)
{
    void* ptr = std::malloc(count);

    if (!ptr)
        throw std::bad_alloc();

    TracyAllocS(ptr, count, 16);
    return ptr;
}

void operator delete(void* ptr) noexcept
{
    if (ptr)
    {
        TracyFreeS(ptr, 16);
        std::free(ptr);
    }
}

void* operator new[](std::size_t count)
{
    void* ptr = std::malloc(count);
    if (!ptr)
        throw std::bad_alloc();
    TracyAllocS(ptr, count, 16);
    return ptr;
}

void operator delete[](void* ptr) noexcept
{
    if (ptr)
    {
        TracyFreeS(ptr, 16);
        std::free(ptr);
    }
}

void operator delete(void* ptr, std::size_t) noexcept
{
    if (ptr)
    {
        TracyFreeS(ptr, 16);
        std::free(ptr);
    }
}

void operator delete[](void* ptr, std::size_t) noexcept
{
    if (ptr)
    {
        TracyFreeS(ptr, 16);
        std::free(ptr);
    }
}

int main()
{
    Testing();
    // return 0;
    N::Engine engine;
    engine.Run();
    return 0;
}
