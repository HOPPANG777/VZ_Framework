#pragma once

class Time
{
	DECLARE_SINGLETON(Time);

public:
	void Update();

	float GetRealDeltaTime() const { return float(deltaTime); }
	float GetDeltaTime() const { return scaledDeltaTime; }
	UINT GetFPS() const { return fps; }
	double GetWorldTime() const { return worldTime; }
	
	void SetTimeScale(float scale) { timeScale = scale; }
	float GetTimeScale() const { return timeScale; }

	void CheckTime() const
	{
		//cout << "FPS : " << fps << ", DeltaTime : " << scaledDeltaTime << ", WorldTime : " << worldTime << '\n';
	}

	// UTC 시간 반환
	SYSTEMTIME GetUTCTime() const
	{
		SYSTEMTIME st;
		::GetSystemTime(&st);  // 현재 로컬 시간을 st에 저장
		return st;
	}

	//// KST(한국 표준시) 시간 반환
	//std::tm GetKSTTime() const
	//{
	//	// UTC 시간 가져오기
	//	std::tm utcTime = GetUTCTime();

	//	// UTC + 9시간 = KST
	//	std::time_t rawTime = std::mktime(&utcTime) + (9 * 60 * 60); // 9시간 추가
	//	std::tm kstTime = *std::gmtime(&rawTime); // gmtime은 타임존 없는 시간 반환

	//	return kstTime;
	//}

	// 컴퓨터 로컬 시간 반환
	SYSTEMTIME GetLocalPCTime() const
	{
		SYSTEMTIME st;
		::GetLocalTime(&st);  // 현재 로컬 시간을 st에 저장
		return st;
	}
	
	//std::tm GetLocalPCTime() const
	//{
	//	SYSTEMTIME systemTime;
	//	GetLocalTime(&systemTime); // 로컬 시간 가져오기

	//	// SYSTEMTIME을 std::tm으로 변환
	//	std::tm localTime = {};
	//	localTime.tm_year = systemTime.wYear - 1900; // tm_year는 1900년부터 시작
	//	localTime.tm_mon = systemTime.wMonth - 1;   // tm_mon은 0부터 시작
	//	localTime.tm_mday = systemTime.wDay;
	//	localTime.tm_hour = systemTime.wHour;
	//	localTime.tm_min = systemTime.wMinute;
	//	localTime.tm_sec = systemTime.wSecond;
	//	localTime.tm_isdst = systemTime.wMilliseconds;

	//	return localTime;
	//}

private:
	chrono::steady_clock::time_point currentTime;
	chrono::steady_clock::time_point lastTime;
	double deltaTime = 0.0;
	double scaledDeltaTime = 0.0;

	UINT fps = 0;
	double worldTime = 0.0;

	UINT frameCount = 0;
	double fpsTimeElapsed = 0.0;

	float scaleDeltaTime = 0.0f;
	float timeScale = 1.0f;		// 타임스케일
};