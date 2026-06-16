#include "cpu_monitor.hpp"

void CPUMon::read_proc_stat(std::vector<CPUStats> &stats)
{
    std::ifstream filestat("/proc/stat");

    std::string line;
    std::getline(filestat, line);

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
void CPUMon::register_cpu_mon()
{
    cpu_mon.start([this]()
                  { read_proc_stat(std::ref(this->stats)); }, std::chrono::seconds(cfg.get<int>(ConfigKey::LifecycleInterval)));
}

void CPUMon::unregister_cpu_mon()
{
    cpu_mon.stop();
}
