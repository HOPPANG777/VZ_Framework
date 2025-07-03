#pragma once

#include "Objects/TextureObject/AnimationRect.h"

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

