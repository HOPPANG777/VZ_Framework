#pragma once

#include "Object.h"

namespace THC
{
	class Sphere : public Object
	{
	public:
		Vector3 center;
		float radius;

	public:
		Sphere(const Vector3& center, const float radius, const Color& color = Color(1))
			: center(center), radius(radius), Object(color)
		{}

	public:
		Hit CheckRayCollision(const Ray& ray) const override
		{
			Hit hit = Hit{ -1.0f, Vector3(), Vector3() };
			
			const float b = 2.0f * (ray.dir.Dot(ray.start - center));
			const float c = (ray.start - center).LengthSquared() - radius * radius;

			const float nabla = b * b * 0.25f - c;

			if (nabla >= 0.0f)
			{
				const float d1 = -b * 0.5f + sqrt(nabla);
				const float d2 = -b * 0.5f - sqrt(nabla);

				hit.distance = min(d1, d2);
				hit.point = ray.start + hit.distance * ray.dir;//시작점에서 단위벡터 방향에 거리를 곱하여 충돌점으로 이동
				(hit.point - center).Normalize(hit.normal);//원의 센터에서 충돌점으로 향하는 벡터를 단위 벡터로 만든다
			}

			/*
			hit.distance = (-2 * ray.dir.Dot(ray.start - center) - sqrt(pow(2 * ray.dir.Dot(ray.start - center),2) - 4*((ray.start - center).LengthSquared() - pow(radius, 2)))) / 2.0f;
			hit.point = ray.start + hit.distance * ray.dir;
			(hit.point - center).Normalize(hit.normal);
			*/

			return hit;
		}
	};
}