#pragma once
// 간단한 레이트레이싱 로직을 담고 있는 클래스

#include "Sphere.h"
#include "Light.h"
#include "Triangle.h"
#include "Square.h"

namespace THC
{
	using namespace std;

	class RayTracer
	{
	public:
		int width, height;
		Light light;
		vector<shared_ptr<Object>> objects;
	public:
		RayTracer(const int width, const int height)
			: width(width), height(height)
		{
			auto sphere1 = make_shared<Sphere>(Vector3(0.0f, 0.0f, 0.6f), 0.4f);

			sphere1->amb = Color(Vector3(0.2f, 0.0f, 0.0f));
			sphere1->diff = Color(1.0f, 0.1f, 0.1f, 1.0f);
			sphere1->spec = Color(Vector3(1.5f));
			sphere1->alpha = 50.0f;

			objects.push_back(sphere1);

			//auto triangle1 = make_shared<Triangle>(Vector3(-2.0f, -2.0f, 2.0f), Vector3(-2.0f, 2.0f, 2.0f), Vector3(2.0f, 2.0f, 2.0f));

			//triangle1->amb = Color(Vector3(0.2f));
			//triangle1->diff = Color(Vector3(0.5f));
			//triangle1->spec = Color(Vector3(0.5f));
			//triangle1->alpha = 5.0f;

			//objects.push_back(triangle1);

			/*vector<Color> texturePixels(4 * 4);
			for (int j = 0; j < 4; ++j)
				for (int i = 0; i < 4; ++i)
				{
					if (i % 4 == 0)
						texturePixels[i + 4 * j] = Color(Vector3(1.0f, 0.0f, 0.0f) * 0.25f * (j + 1.0f));
					else if (i % 4 == 1)
						texturePixels[i + 4 * j] = Color(Vector3(0.0f, 1.0f, 0.0f) * 0.25f * (j + 1.0f));
					else if (i % 4 == 2)
						texturePixels[i + 4 * j] = Color(Vector3(0.0f, 0.0f, 1.0f) * 0.25f * (j + 1.0f));
					else
						texturePixels[i + 4 * j] = Color(Vector3(1.0f, 1.0f, 1.0f) * 0.25f * (j + 1.0f));
				}
				*/

			//auto imageTexture = make_shared<Texture>("_Textures/RocksImage.jpg");

			/*vector<Color> texturePixels(4 * 4);
			for (int j = 0; j < 4; ++j)
				for (int i = 0; i < 4; ++i)
				{
					if (i % 4 == 0)
						texturePixels[i + 4 * j] = Color(Vector3(1.0f, 0.0f, 0.0f) * 0.25f * (1.0f + j));
					else if (i % 4 == 1)
						texturePixels[i + 4 * j] = Color(Vector3(0.0f, 1.0f, 0.0f) * 0.25f * (1.0f + j));
					else if (i % 4 == 2)
						texturePixels[i + 4 * j] = Color(Vector3(0.0f, 0.0f, 1.0f) * 0.25f * (1.0f + j));
					else
						texturePixels[i + 4 * j] = Color(Vector3(1.0f, 1.0f, 1.0f) * 0.25f * (1.0f + j));
				}

			auto imageTexture = make_shared<Texture>(4, 4, texturePixels);
			*/

			float uvRatio = 1.0f;
			auto square = make_shared<Square>(Vector3(-2.0f, 2.0f, 2.0f), Vector3(2.0f, 2.0f, 2.0f), Vector3(2.0f, -2.0f, 2.0f), Vector3(-2.0f, -2.0f, 2.0f),
				Vector2(), Vector2(uvRatio, 0.0f), Vector2(uvRatio, uvRatio), Vector2(0, uvRatio));
			square->amb = Color(Vector3(0.2f));
			square->diff = Color(Vector3(1.0f));
			square->spec = Color(Vector3(0.0f));

			//square->ambTexture = imageTexture;
			//square->difTexture = imageTexture;

			objects.push_back(square);

			light = Light{ Vector3(0.0f, 1.0f, 0.2f) };
		}

		Vector3 TransformScreenToWorld(const Vector2& screenPos) const
		{
			float aspectRatio = float(width) / height;

			float transformedX = aspectRatio * (screenPos.x / (width - 1) * 2.0f - 1.0f);
			float transformedY = -screenPos.y / (height - 1) * 2.0f + 1.0f;

			return Vector3(transformedX, transformedY, 0.0f);
		}

		Hit FindClosestCollision(const Ray& ray) const
		{
			float closestD = 1000.0f;
			Hit closestHit = Hit{ -1.0f, Vector3(), Vector3() };

			for (const auto& obj : objects)
			{
				auto hit = obj->CheckRayCollision(ray);

				if (hit.distance >= 0.0f)
				{
					if (hit.distance < closestD)
					{
						hit.obj = obj;
						closestHit = hit;
						closestD = closestHit.distance;

						closestHit.uv = hit.uv;
					}
				}
			}

			return closestHit;
		}

		Color TraceRay(const Ray& ray) const
		{
			const Hit hit = FindClosestCollision(ray);

			if (hit.distance >= 0.0f)
			{
				Color pointColor;	

				if (hit.obj->ambTexture)
				{
					pointColor = hit.obj->amb * hit.obj->ambTexture->SampleLinear(hit.uv);
				}
				else
				{
					pointColor = Color(hit.obj->amb);
				}

				// Diffuse
				const Vector3 n = hit.normal;
				Vector3 l;
				(light.pos - hit.point).Normalize(l);


				/*
				* ±×¸²ÀÚ Off
				Ray shadowRay = { hit.point + 1e-4f * l, l };
				

				const auto& shadowHit = FindClosestCollision(shadowRay);

				//if(shadowHit.distance>=0 && shadowHit.distance < l.Distance(hit.point, light.pos))
				if(shadowHit.obj != nullptr && shadowHit.distance <= (hit.point - light.pos).Length())
					return Color(hit.obj->amb);
				*/
				const float diff = max(n.Dot(l), 0.0f);

				if (hit.obj->difTexture)
				{
					pointColor += hit.obj->diff * diff * hit.obj->difTexture->SampleLinear(hit.uv);
				}
				else
				{
					pointColor += Color(hit.obj->diff * diff);
				}

				//Vector3 r;
				//r = 2 * hit.normal.Dot(l) * hit.normal - l;
				const Vector3 r = 2.0f * n.Dot(l) * n - l;

				//const float spec = pow(max((-ray.dir).Dot(r), 0.0f), sphere->alpha);
				const float spec = pow(max((-ray.dir).Dot(r), 0.0f), hit.obj->alpha);

				pointColor += Color(hit.obj->spec * spec);

				//return Color(sphere->amb + sphere->diff);
				//return Color(sphere->diff * diff);
				//return Color(sphere->spec * spec * sphere->ks);
				return pointColor;
			}


			return Color();
		}

		Color TraceRay2x2(const Vector3& eyePos, Vector3 pixelPos, const float dx, const int recursiveLevel) const
		{

		}

		void Render(vector<Color>& pixels) const
		{

/*			ImGui::Begin("Sphere");
			ImGui::SliderFloat3("Center", (float*)&sphere->center, -(float)width / height, (float)width / height);
			ImGui::SliderFloat("Radius", (float*)&sphere->radius, 0.0f, 1.0f);
			ImGui::SliderFloat3("RGB", (float*)&sphere->color, 0.0f, 1.0f);
			ImGui::End();
			*/
			ImGui::Begin("Scene Control");
			ImGui::SliderFloat3("Light Position", (float*)&light.pos, -2.0f, 2.0f);
			//ImGui::SliderFloat3("Sphere Position", (float*)&sphere->center, -1.0f, 1.0f);
			ImGui::End();
			//const Vector3 eyePos(0.0f, 0.0f, -1.5f);
			Vector3 eyePos(0.0f, 0.0f, -1.5f);


			fill(pixels.begin(), pixels.end(), Color());



#pragma omp parallel for
			for (int y = 0; y < height; ++y)
				for (int x = 0; x < width; ++x)
				{
					const Vector3 pixelWorldPos = TransformScreenToWorld(Vector2(float(x), float(y)));


					auto rayDir = pixelWorldPos - eyePos;
					rayDir.Normalize();

					Ray pixelRay{ pixelWorldPos, rayDir };

					pixels[size_t(x + width * y)] = TraceRay(pixelRay);
				}
		}
	};
}