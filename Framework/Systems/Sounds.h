#pragma once

using namespace FMOD;

class Sounds
{
	DECLARE_SINGLETON(Sounds);
public:
	void Init();

	void Update();

	void AddSound(string name, wstring file, bool loop = false);

	void Play(string name, float volum = 1.0f);
	void Stop(string name);
	void Pause(string name);
	void Resum(string name);

	bool IsPlaying(string name);
	bool IsPause(string name);

	void SetVolume(string name, float volum);
	float GetVolume(string name);

private:
	System* system;
	Sound** sound;
	Channel** channel;

	map<string, Sound**> sounds;
	map<string, Sound**>::iterator iter;

	UINT count = 0;
	float volum = 1.0f;
};

