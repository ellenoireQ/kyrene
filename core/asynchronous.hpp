#pragma once

#include <thread>
#include <future>
#include <functional>

class Async
{
public:
    template <typename Func, typename... Args>
    static auto run(Func &&func, Args &&...args)
        -> std::future<std::invoke_result_t<Func, Args...>>
    {
        return std::async(
            std::launch::async,
            std::forward<Func>(func),
            std::forward<Args>(args)...);
    }
};