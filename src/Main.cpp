#include "Core/InnerCore/Engine.hpp"
#include "sketch.hpp"

static void Testing()
{

    Sketch::FreeList<unsigned int> fl;

    fl.Push(1);
    fl.Push(2);
    fl.Push(3);
    fl.Push(5);
    fl.Push(22);
    fl.Push(20);
    fl.Push(35);
    fl.Push(100);
    fl.Erase(3);
    fl.Erase(6);

    fl.Push(1000);
    fl.Push(500);
    fl.Push(1500);

    N::U::Log::Print(fl);

    // fl.Remove(1);
}
int main()
{
    Testing();
    return 0;
    N::Engine engine;
    engine.Run();
    return 0;
}
