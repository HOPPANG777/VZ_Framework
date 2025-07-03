#include "Framework.h"
#include "Time.h"

Time::Time()
{
	currentTime = std::chrono::steady_clock::now();
	lastTime = currentTime;
}

Time::~Time() {}

void Time::Update()
{
	lastTime = currentTime;

	currentTime = std::chrono::steady_clock::now();

	std::chrono::duration<double> delta = currentTime - lastTime;
	deltaTime = delta.count();

	worldTime += deltaTime;

	fpsTimeElapsed += deltaTime;
	++frameCount;

	if (fpsTimeElapsed >= 1.0)
	{
		fps = frameCount;
		frameCount = 0;
		fpsTimeElapsed = 0.0;

		CheckTime();
	}

	scaledDeltaTime = float(deltaTime * timeScale);
}
