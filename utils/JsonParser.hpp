#pragma once

#include <nlohmann/json.hpp>

#include <fstream>
#include <optional>
#include <sstream>
#include <stdexcept>
#include <string>

namespace kyrene::utils
{
    class JsonParser
    {
    public:
        using Json = nlohmann::json;

        static Json parseString(const std::string &text)
        {
            return Json::parse(text);
        }

        static Json parseFile(const std::string &filePath)
        {
            std::ifstream input(filePath);
            if (!input.is_open())
            {
                throw std::runtime_error("Failed to open JSON file: " + filePath);
            }

            std::stringstream buffer;
            buffer << input.rdbuf();
            return parseString(buffer.str());
        }

        static std::optional<Json> tryParseFile(const std::string &filePath, std::string *errorMessage = nullptr)
        {
            try
            {
                return parseFile(filePath);
            }
            catch (const std::exception &ex)
            {
                if (errorMessage != nullptr)
                {
                    *errorMessage = ex.what();
                }
                return std::nullopt;
            }
        }
    };
}
