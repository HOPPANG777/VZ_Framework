#pragma once

#include "Framework.h"

class ServiceLocator
{
public:
    template<typename T>
    static void Register(const std::string& key, std::shared_ptr<T> service)
    {
        services()[key] = service;
    }

    template<typename T>
    static std::shared_ptr<T> Get(const std::string& key)
    {
        auto it = services().find(key);
        if (it == services().end())
        {
            throw std::runtime_error("Service not found: " + key);
        }
        return std::static_pointer_cast<T>(it->second);
    }

    static void Clear()
    {
        services().clear();
    }

private:
    static std::unordered_map<std::string, std::shared_ptr<void>>& services()
    {
        static std::unordered_map<std::string, std::shared_ptr<void>> instance;
        return instance;
    }
};