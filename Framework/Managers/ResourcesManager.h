#pragma once
#include <mutex>
#include <string>
#include <unordered_set>
#include <vector>

class ResourcesManager
{
	DECLARE_SINGLETON(ResourcesManager);

public:
    // 단순 시뮬레이션: 각 리소스 당 100ms 소요
    void LoadResource(const std::string& resource);
    bool IsResourceLoaded(const std::string& resource);
    bool AreResourcesLoaded(const std::vector<std::string>& resources);

private:
    std::mutex mtx;
    std::unordered_set<std::string> loadedResources;

};

