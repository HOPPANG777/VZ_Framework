#pragma once
#include "../Framework.h"


class ResourcesManager
{
	DECLARE_SINGLETON(ResourcesManager);

public:
    // 단순 시뮬레이션: 각 리소스 당 100ms 소요
    void LoadResource(const string& resource);
    bool IsResourceLoaded(const string& resource);
    bool AreResourcesLoaded(const vector<string>& resources);

private:
    mutex mtx;
    unordered_set<string> loadedResources;

};

