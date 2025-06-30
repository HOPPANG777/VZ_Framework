#pragma once
#include <string>

class Wind : public AnimationRect
{
public:
	Wind(const Vector3& position, const Vector3& scale, const float rotation, const wstring path = L"");
	~Wind();

public:
	void Update() override;
	void Render() override;

private:


};

