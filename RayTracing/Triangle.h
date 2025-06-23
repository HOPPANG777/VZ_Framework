#pragma once

#include "Object.h"

namespace THC
{
	class Triangle : public Object
	{
	public:
		Vector3 v0, v1, v2;
		Vector2 uv0, uv1, uv2;
		Vector3 normal;
		Vector3 p;
	public:
		Triangle() : v0(Vector3()), v1(Vector3()), v2(Vector3())
		{
		}

		Triangle(const Vector3& v0, const Vector3& v1, const Vector3& v2,
			const Vector2& uv0, const Vector2& uv1, const Vector2& uv2)
			: v0(v0), v1(v1), v2(v2), uv0(uv0), uv1(uv1), uv2(uv2)
		{
		}

		Hit CheckRayCollision(const Ray& ray) const override
		{
			Hit hit = Hit{ -1.0f, Vector3(), Vector3() };

			Vector3 point, faceNormal;
			float t, w0, w1;

			if (IntersectRayTriangle(ray.start, ray.dir, v0, v1, v2, point, faceNormal, t, w0, w1))
			{
				hit.distance = t;
				hit.point = point;
				hit.normal = faceNormal;

				hit.uv = uv0 * w0 + uv1 * w1 + uv2 * (1.0f - w0 - w1);
			}

			return hit;
		}

		bool IntersectRayTriangle(const Vector3& origin, const Vector3& dir,
			const Vector3& v0, const Vector3& v1, const Vector3& v2,
			Vector3& point, Vector3& faceNormal, float& t,
			float& w0, float& w1) const
		{
			((v1 - v0).Cross(v2 - v0)).Normalize(faceNormal);

			if (-dir.Dot(faceNormal) < 0.0f) return false;

			if (abs(dir.Dot(faceNormal)) < 1e-2f) return false;

			t = (v0.Dot(faceNormal) - origin.Dot(faceNormal)) / dir.Dot(faceNormal);
			
			if (t < 0.0f) return false;

			point = origin + t * dir;

			const Vector3 cross0 = (v2 - v1).Cross(point - v1);
			const Vector3 cross1 = (v0 - v2).Cross(point - v2);
			const Vector3 cross2 = (v1 - v0).Cross(point - v0);

			if (faceNormal.Dot(cross0) < 0.0f || faceNormal.Dot(cross1) < 0.0f || faceNormal.Dot(cross2) < 0.0f) return false;

			const float area0 = cross0.Length();
			const float area1 = cross1.Length();
			const float area2 = cross2.Length();

			const float areaSum = area0 + area1 + area2;

			w0 = area0 / areaSum;
			w1 = area1 / areaSum;

			//area0, 1, 2를 구하고
			//areaSum을 구하고
			//w0, w1을 구함

			return true;
		}
		//Vector3 normal = (v1-v0).Cross(v2-v0).Normalize()
	};
}