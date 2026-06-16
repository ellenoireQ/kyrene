#include "cpu_monitor.hpp"

void CPUMon::read_proc_stat(std::vector<CPUStats> &stats)
{
    std::ifstream filestat("/proc/stat");

    std::string line;

    std::vector<CPUStats> temp_stats;

    while (std::getline(filestat, line))
    {
        if (line.rfind("cpu", 0) == 0)
        {
            std::istringstream iss(line);

            std::string cpu_name;
            unsigned int user;
            unsigned int nice;
            unsigned int system;
            unsigned int idle;

            iss >> cpu_name >> user >> nice >> system >> idle;

            temp_stats.push_back(CPUStats{
                .cpu_name = cpu_name,
                .user = user,
                .nice = nice,
                .system = system,
                .idle = idle,
            });
        }
    }
    std::lock_guard<std::mutex> lock(mutex);
    previous_stats = stats;
    stats = std::move(temp_stats);
}

int CPUMon::total(int index)
{
    std::lock_guard<std::mutex> lock(mutex);

    return stats[index].idle +
           stats[index].nice +
           stats[index].system +
           stats[index].user;
}

double CPUMon::calculate_usage(size_t index)
{
    std::lock_guard<std::mutex> lock(mutex);

    if (previous_stats.empty() || stats.empty())
    {
        return 0.0;
    }

    uint64_t old_total =
        previous_stats[index].user +
        previous_stats[index].nice +
        previous_stats[index].system +
        previous_stats[index].idle;

    uint64_t new_total =
        stats[index].user +
        stats[index].nice +
        stats[index].system +
        stats[index].idle;

    uint64_t old_idle =
        previous_stats[index].idle;

    uint64_t new_idle =
        stats[index].idle;

    uint64_t total_delta =
        new_total - old_total;

    uint64_t idle_delta =
        new_idle - old_idle;

    double result =
        100.0 *
        (total_delta - idle_delta) /
        total_delta;

    return result;
}
void CPUMon::register_cpu_mon()
{
    cpu_mon.start([this]()
                  { read_proc_stat(std::ref(this->stats)); }, std::chrono::seconds(cfg.get<int>(ConfigKey::LifecycleInterval)));
}

void CPUMon::unregister_cpu_mon()
{
    cpu_mon.stop();
}
