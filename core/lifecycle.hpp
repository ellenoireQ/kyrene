#define TAG "lifecycle"

#include <concepts>
#include <stop_token>
#include <thread>
#include <chrono>
#include <functional>
#include <core/logger.hpp>

class Lifecycle
{
private:
    std::jthread worker;
    std::atomic<bool> running = false;

public:
    template <typename Func>
    void start(
        Func &&func,
        std::chrono::milliseconds interval)
    {
        worker = std::jthread(
            [this, func = std::forward<Func>(func),
             interval](std::stop_token token)
            {
                running = true;
                while (!token.stop_requested())
                {
                    std::invoke(func);
                    std::this_thread::sleep_for(interval);
                    LOG_INFO(TAG, std::format("Worker running {}", typeid(func).name()));
                }
                running = false;
            });
    }

    void stop()
    {
        worker.request_stop();
        running = false;
    }

    bool is_running()
    {
        return running;
    }
};