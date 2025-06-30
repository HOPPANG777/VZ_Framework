#pragma once
#include <string>

class UnitBase : public AnimationRect
{
private:


public:
	UnitBase(const Vector3& position, const Vector3& scale, const float rotation, const wstring& path);
	~UnitBase();

public:
	void Update();
	void Render();

};

