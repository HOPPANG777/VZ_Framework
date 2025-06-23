#pragma once

#include <iostream>
#include <directxtk/SimpleMath.h>

namespace THC//≥ª ¿Ã∏ß
{
	using namespace DirectX;
	using namespace SimpleMath;

	class Hit
	{
	public:
		float distance;
		Vector3 point;
		Vector3 normal;
		Vector2 uv;
		
		shared_ptr<class Object> obj;
	};
}