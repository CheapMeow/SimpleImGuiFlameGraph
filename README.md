# SimpleImGuiFireGraph

## Build

```shell
cmake -S . -B build
cmake --build build/
```

## Example

Running the `SimpleImGuiFireGraphExample` example will display an example window showcasing the flame graph widget. The code is available for reference in `example/main.cpp`.

```cpp
TestFun();
FireGraphDrawer::Draw(TimerSingleton::Get().GetScopeTimes(),
                        TimerSingleton::Get().GetMaxDepth(),
                        TimerSingleton::Get().GetGlobalStart());
```

![](./fire_graph.gif)

## Advantages

1.Time measurement and flame graph drawing are separated.

Example:

```cpp
TestFun();
FireGraphDrawer::Draw(TimerSingleton::Get().GetScopeTimes(),
                        TimerSingleton::Get().GetMaxDepth(),
                        TimerSingleton::Get().GetGlobalStart());
```

Implementation code of `FireGraphDrawer::Draw` is simple. Less than 100 lines.

2.Time measurement can be used in any scope, only one line of macro code is needed.

Example:

```cpp
void TestFun()
{
    FUNCTION_TIMER();

    std::random_device              rd;        // Will be used to obtain a seed for the random number engine
    std::mt19937                    gen(rd()); // Standard mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<> dis(1, 3000);

    {
        SCOPE_TIMER("Test Scope 1");

        int count = dis(gen);
        for (int i = 0; i < count; i++)
        {
            int x = dis(gen);
        }

        {
            SCOPE_TIMER("Test Scope 2");

            int count2 = dis(gen);
            for (int i = 0; i < count2; i++)
            {
                int x = dis(gen);
            }
        }

        {
            SCOPE_TIMER("Test Scope 3");

            int count3 = dis(gen);
            for (int i = 0; i < count3; i++)
            {
                int x = dis(gen);
            }
        }
    }

    {
        SCOPE_TIMER("Test Scope 4");

        int count = dis(gen);
        for (int i = 0; i < count; i++)
        {
            int x = dis(gen);
        }
    }
}
```

## Reference

`Timer` class refers to [https://github.com/TheCherno/Hazel](https://github.com/TheCherno/Hazel)

`FireGraphDrawer` class refers to [https://github.com/bwrsandman/imgui-flame-graph](https://github.com/bwrsandman/imgui-flame-graph)