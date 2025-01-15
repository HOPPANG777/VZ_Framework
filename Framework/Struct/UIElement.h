#pragma once
//#include "Framework.h"

struct UIElement
{
	float xRatio; // 화면 너비 기준 위치 비율 (0.0 ~ 1.0)
	float yRatio; // 화면 높이 기준 위치 비율 (0.0 ~ 1.0)
	float widthRatio;  // 너비 비율
	float heightRatio; // 높이 비율

	// 생성자
	UIElement(float x, float y, float width, float height) : xRatio(x), yRatio(y), widthRatio(width), heightRatio(height) {}

	RECT GetRect(float screenWidth, float screenHeight)
	{
		RECT rect;
		rect.left = LONG(xRatio * screenWidth);
		rect.top = LONG(yRatio * screenHeight);
		rect.right = LONG(rect.left + (widthRatio * screenWidth));
		rect.bottom = LONG(rect.top + (heightRatio * screenHeight));
		return rect;
	}
};