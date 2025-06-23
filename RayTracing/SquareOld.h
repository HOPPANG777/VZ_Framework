#pragma once

#include "Object.h"

namespace THC
{
	class Square : public Object
	{
	public:
		Vector3 v0, v1, v2, v3;
		Vector3 normal;
		Vector3 p;
	public:
		Square() : v0(Vector3()), v1(Vector3()), v2(Vector3())
		{
		}

		Square(const Vector3& v0, const Vector3& v1, const Vector3 v2, const Vector3 v3)
			: v0(v0), v1(v1), v2(v2), v3(v3)
		{
		}

		Hit CheckRayCollision(const Ray& ray) const override
		{
			Hit hit = Hit{ -1.0f, Vector3(), Vector3() };

			Vector3 point, faceNormal;
			float t;

			if (IntersectRaySquare(ray.start, ray.dir, v0, v1, v2, v3, point, faceNormal, t))
			{
				hit.distance = t;
				hit.point = point;
				hit.normal = faceNormal;
			}

			return hit;
		}

		bool IntersectRaySquare(const Vector3& origin, const Vector3& dir,
			const Vector3& v0, const Vector3& v1, const Vector3& v2, const Vector3& v3,
			Vector3& point, Vector3& faceNormal, float& t) const
		{
			((v1 - v0).Cross(v2 - v0)).Normalize(faceNormal);

			if (-dir.Dot(faceNormal) < 0.0f) return false;

			if (abs(dir.Dot(faceNormal)) < 1e-2f) return false;

			t = (v0.Dot(faceNormal) - origin.Dot(faceNormal)) / dir.Dot(faceNormal);

			if (t < 0.0f) return false;

			point = origin + t * dir;

			const Vector3 normal0 = (v1 - v0).Cross(point - v0);
			const Vector3 normal1 = (v2 - v1).Cross(point - v1);
			const Vector3 normal2 = (v3 - v2).Cross(point - v2);
			const Vector3 normal3 = (v0 - v3).Cross(point - v3);

			if (faceNormal.Dot(normal0) < 0.0f || faceNormal.Dot(normal1) < 0.0f || faceNormal.Dot(normal2) < 0.0f || faceNormal.Dot(normal3) < 0.0f) return false;

			return true;
		}
		//Vector3 normal = (v1-v0).Cross(v2-v0).Normalize()
	};
}