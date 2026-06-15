#pragma once

#include <unordered_map>
#include <variant>
#include <string>

enum class ConfigKey
{
    LifecycleInterval,
};

using ConfigValue = std::variant<int, bool, std::string>;

class AppConfig
{
private:
    std::unordered_map<ConfigKey, ConfigValue> config;

public:
    template <typename T>
    void set(ConfigKey key, T value)
    {
        config[key] = value;
    }

    template <typename T>
    T get(ConfigKey key) const
    {
        return std::get<T>(config.at(key));
    }
};