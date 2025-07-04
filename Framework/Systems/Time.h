#pragma once
#include "Framework.h"

class Time
{
	DECLARE_SINGLETON(Time);

public:
	void Update();

	float GetRealDeltaTime() const { return float(deltaTime); }
	float GetDeltaTime() const { return float(scaledDeltaTime); }
	UINT GetFPS() const { return fps; }
	float GetWorldTime() const { return float(worldTime); }
	
	void SetTimeScale(float scale) { timeScale = scale; }
	float GetTimeScale() const { return timeScale; }

	void CheckTime() const
	{
		std::cout << "FPS : " << fps << ", DeltaTime : " << scaledDeltaTime << ", WorldTime : " << worldTime << '\n';
	}

	// UTC 시간 반환
	SYSTEMTIME GetUTCTime() const
	{
		SYSTEMTIME st;
		::GetSystemTime(&st);  // 현재 로컬 시간을 st에 저장
		return st;
	}

	// 컴퓨터 로컬 시간 반환
	SYSTEMTIME GetLocalPCTime() const
	{
		SYSTEMTIME st;
		::GetLocalTime(&st);  // 현재 로컬 시간을 st에 저장
		return st;
	}

private:
	std::chrono::steady_clock::time_point currentTime;
	std::chrono::steady_clock::time_point lastTime;
	double deltaTime = 0.0;
	double scaledDeltaTime = 0.0;

	UINT fps = 0;
	double worldTime = 0.0;

	UINT frameCount = 0;
	double fpsTimeElapsed = 0.0;

	float scaleDeltaTime = 0.0f;
	float timeScale = 1.0f;		// 타임스케일
};