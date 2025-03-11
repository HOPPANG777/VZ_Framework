#pragma once

class LumberjackPlayer : public AnimationRect
{
private:


public:
	LumberjackPlayer(const Vector3& position, const Vector3& scale, const float rotation, const wstring& path);
	~LumberjackPlayer();

public:
	void Update();
	void Render();

};

