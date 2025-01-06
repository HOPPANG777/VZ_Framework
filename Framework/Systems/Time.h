#pragma once

class Time
{
	DECLARE_SINGLETON(Time);

public:
	void Update();

	float GetDeltaTime() const { return float(deltaTime); }
	UINT GetFPS() const { return fps; }
	double GetWorldTime() const { return worldTime; }
	
	void CheckTime() const
	{
		//cout << "FPS : " << fps << ", DeltaTime : " << deltaTime 
		//	<< ", WorldTime : " << worldTime << '\n';

		cout << "Hour : " << GetLocalPCTime().tm_hour << ", Min : " 
			<< GetLocalPCTime().tm_min << ", Sec : " << GetLocalPCTime().tm_sec << '\n';
	}

	// UTC 시간 반환
	std::tm GetUTCTime() const
	{
		SYSTEMTIME systemTime;
		GetSystemTime(&systemTime); // UTC 시간 가져오기

		// SYSTEMTIME을 std::tm으로 변환
		std::tm utcTime = {};
		utcTime.tm_year = systemTime.wYear - 1900; // tm_year는 1900년부터 시작
		utcTime.tm_mon = systemTime.wMonth - 1;   // tm_mon은 0부터 시작
		utcTime.tm_mday = systemTime.wDay;
		utcTime.tm_hour = systemTime.wHour;
		utcTime.tm_min = systemTime.wMinute;
		utcTime.tm_sec = systemTime.wSecond;

		return utcTime;
	}

	// KST(한국 표준시) 시간 반환
	std::tm GetKSTTime() const
	{
		// UTC 시간 가져오기
		std::tm utcTime = GetUTCTime();

		// UTC + 9시간 = KST
		std::time_t rawTime = std::mktime(&utcTime) + (9 * 60 * 60); // 9시간 추가
		std::tm kstTime = *std::gmtime(&rawTime); // gmtime은 타임존 없는 시간 반환

		return kstTime;
	}

	// 컴퓨터 로컬 시간 반환
	std::tm GetLocalPCTime() const
	{
		SYSTEMTIME systemTime;
		GetLocalTime(&systemTime); // 로컬 시간 가져오기

		// SYSTEMTIME을 std::tm으로 변환
		std::tm localTime = {};
		localTime.tm_year = systemTime.wYear - 1900; // tm_year는 1900년부터 시작
		localTime.tm_mon = systemTime.wMonth - 1;   // tm_mon은 0부터 시작
		localTime.tm_mday = systemTime.wDay;
		localTime.tm_hour = systemTime.wHour;
		localTime.tm_min = systemTime.wMinute;
		localTime.tm_sec = systemTime.wSecond;

		return localTime;
	}

private:
	chrono::steady_clock::time_point currentTime;
	chrono::steady_clock::time_point lastTime;
	double deltaTime = 0.0;

	UINT fps = 0;
	double worldTime = 0.0;

	UINT frameCount = 0;
	double fpsTimeElapsed = 0.0;
};