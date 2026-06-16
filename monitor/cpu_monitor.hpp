#include <iostream>
#include <fstream>
#include <sstream>
#include <stop_token>
#include <thread>
#include <vector>
#include <mutex>
#include <string>
#include <core/lifecycle.hpp>
#include <config/app_config.hpp>

struct CPUStats
{
    std::string cpu_name;
    uint64_t user;
    uint64_t nice;
    uint64_t system;
    uint64_t idle;
};

class CPUMon
{
private:
    std::mutex mutex;
    std::vector<CPUStats> stats;
    std::vector<CPUStats> previous_stats;
    Lifecycle cpu_mon;
    void read_proc_stat(std::vector<CPUStats> &stats);
    AppConfig &cfg;

public:
    explicit CPUMon(AppConfig &cfg)
        : cfg(cfg) {
          };
    void register_cpu_mon();
    void unregister_cpu_mon();
    int total(int index);

    double calculate_usage(size_t index);
    std::vector<CPUStats> get_data()
    {
        std::lock_guard<std::mutex> lock(mutex);
        return stats;
    };
    bool is_running()
    {
        return cpu_mon.is_running();
    }
};