#pragma once

#include "Hit.h"
#include "Ray.h"
#include "Texture.h"

namespace THC
{
	class Object
	{
	public:
		Color amb;//ambient
		Color diff;//diffuse
		Color spec;//specular
		float alpha = 0.0f;

		shared_ptr<Texture> ambTexture;
		shared_ptr<Texture> difTexture;

	public:
		Object(const Color& color = Color(1))
			: amb(color), diff(color), spec(color)
		{}

	public:
		virtual Hit CheckRayCollision(const Ray& ray) const = 0;
	};
}