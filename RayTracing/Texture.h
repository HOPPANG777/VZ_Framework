#pragma once

#include <stb_image.h>

namespace THC
{
	class Texture
	{
	public:
		int width = 0, height = 0, channels = 0;
		vector<uint8_t> image;

		Texture(const string& fileName)
		{
			unsigned char* img = stbi_load(fileName.c_str(), &width, &height, &channels, 0);

			image.resize(size_t(width * height * channels));
			memcpy(image.data(), img, image.size() * sizeof(image[0]));

			delete img;
		}

		Texture(const int width, const int height, const vector<Color>& pixels)
			: width(width), height(height), channels(3)
		{
			image.resize(size_t(width * height * channels));

			for (int j = 0; j < height; ++j)
				for (int i = 0; i < width; ++i)
				{
					const auto& color = pixels[size_t(i + j * width)];

					image[(i + j * width) * channels + 0] = uint8_t(color.x * 255);
					image[(i + j * width) * channels + 1] = uint8_t(color.y * 255);
					image[(i + j * width) * channels + 2] = uint8_t(color.z * 255);
				}
		}

		Color GetClamped(int i, int j)
		{
			i = clamp(i, 0, width - 1);
			j = clamp(j, 0, height - 1);

			const float r = image[(i + j * width) * channels + 0] / 255.0f;
			const float g = image[(i + j * width) * channels + 1] / 255.0f;
			const float b = image[(i + j * width) * channels + 2] / 255.0f;

			return Color(r, g, b);
		}

		Color GetWrapped(int i, int j)
		{
			i %= width;
			j %= height;

			for (;i < 0;)
				i += width;
			for (;j < 0;)
				j += height;

			const float r = image[(i + j * width) * channels + 0] / 255.0f;
			const float g = image[(i + j * width) * channels + 1] / 255.0f;
			const float b = image[(i + j * width) * channels + 2] / 255.0f;

			return Color(r, g, b);
		}

		Color InterpolateBilinear(const float dx, const float dy,
			const Color& c00, const Color& c10, const Color& c01, const Color& c11)
		{
			Color a = c00 * (1.0f - dx) + c10 * dx;
			a.w = 1.0f;
			Color b = c01 * (1.0f - dx) + c11 * dx;
			b.w = 1.0f;

			return a * (1 - dy) + b * dy;
		}

		Color SamplePoint(const Vector2& uv)
		{
			//x와 y를 구하고 이를 통해 i와 j를 구하라 그 이후 wrap이나  clamp를 써서 색을 구하면 된다
			//const Vector2 xy = Vector2(uv.x * width - 0.5f, uv.y * height - 0.5f);
			const Vector2 xy = uv * Vector2(float(width), float(height)) - Vector2(0.5f);
			const int i = int(round(xy.x));
			const int j = int(round(xy.y));
			
			//return GetClamped(round(xy.x), round(xy.y));
			return GetWrapped(i, j);
		}

		Color SampleLinear(const Vector2& uv)
		{
			//const float x = uv.x * width - 0.5f;
			//const float y = uv.y * height - 0.5f;
			const Vector2 xy = uv * Vector2(float(width), float(height)) - Vector2(0.5f);

			const int i = int(floor(xy.x));
			const int j = int(floor(xy.y));

			const float dx = xy.x - i;
			const float dy = xy.y - j;

			return InterpolateBilinear(dx, dy, GetWrapped(i,j), GetWrapped(i+1, j), GetWrapped(i, j+1), GetWrapped(i+1, j+1));
		}
	};
}