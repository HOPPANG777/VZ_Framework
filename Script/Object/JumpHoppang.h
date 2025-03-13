	#pragma once

#include "Objects/TextureObject/AnimationRect.h"

class JumpHoppang : public AnimationRect
{
public:
	JumpHoppang(const Vector3& position, const Vector3& scale, const float rotation, const wstring& path);
	~JumpHoppang();

public:
	void Update();
	void Render();

};

