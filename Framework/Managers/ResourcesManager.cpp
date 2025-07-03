#include "Framework.h"
#include "ResourcesManager.h"

ResourcesManager::ResourcesManager() {}

ResourcesManager::~ResourcesManager() {}

void ResourcesManager::LoadResource(const std::string& resource)
{
	// 이미 로드된 경우는 넘어감
	{
		std::lock_guard<std::mutex> lock(mtx);
		if (loadedResources.find(resource) != loadedResources.end()) return;
	}

	// 실제 로딩 작업 (예: 파일 읽기, 디코딩 등)
	std::this_thread::sleep_for(std::chrono::milliseconds(100));

	// 로딩 완료 후 기록
	{
		std::lock_guard<std::mutex> lock(mtx);
		loadedResources.insert(resource);
	}
}

bool ResourcesManager::IsResourceLoaded(const std::string& resource)
{
	std::lock_guard<std::mutex> lock(mtx);
	return (loadedResources.find(resource) != loadedResources.end());
}

bool ResourcesManager::AreResourcesLoaded(const std::vector<std::string>& resources)
{
	for (const auto& res : resources)
	{
		if (!IsResourceLoaded(res))
			return false;
	}
	return true;
}
