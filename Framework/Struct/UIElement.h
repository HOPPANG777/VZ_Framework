#pragma once

//==============================================================================
// UIElement::예제
//==============================================================================
// 
//      화면 크기: 1920x1080
// 
//      UI 요소 설정:
//      xRatio      = 0.1   →   요소의 왼쪽 위치가 화면 너비의 10 % .
//      yRatio      = 0.2   →   요소의 위쪽 위치가 화면 높이의 20 % .
//      widthRatio  = 0.3   →   요소의 너비가 화면 너비의 30 % .
//      heightRatio = 0.1   →   요소의 높이가 화면 높이의 10 % .
//
//      좌측(left) :
//      rect.left = xRatio * screenWidth = 0.1 * 1920 = 192
//
//      상단(top) :
//      rect.top = yRatio * screenHeight = 0.2 * 1080 = 216
//
//      우측(right) :
//      rect.right = rect.left + (widthRatio * screenWidth) = 192 + (0.3 * 1920) = 192 + 576 = 768
//
//      하단(bottom) :
//      rect.bottom = rect.top + (heightRatio * screenHeight) = 216 + (0.1 * 1080) = 216 + 108 = 324
//
//      화면 전체 크기(1920x1080)
//      ┌──────────────────────────────────────────────────────────┐
//      │                                                          │
//      │    rect.top(216)                                         │
//      │    ┌───────────────────────────────┐                     │
//      │    │                               │                     │
//      │    │                               │ rect.bottom(324)    │
//      │    │                               │                     │
//      │    │         UI Element            │                     │
//      │    │                               │                     │
//      │    │ rect.left(192)                │                     │
//      │    └───────────────────────────────┘                     │
//      │              rect.right(768)                             │
//      │                                                          │
//      └──────────────────────────────────────────────────────────┘
//
//==============================================================================

struct UIElement
{
    // 화면의 너비와 높이를 기준으로 요소의 위치와 크기를 비율로 표현
    float xRatio;       // 요소의 왼쪽 위치 비율 (0.0 ~ 1.0) → 화면 너비(screenWidth)의 비율
    float yRatio;       // 요소의 위쪽 위치 비율 (0.0 ~ 1.0) → 화면 높이(screenHeight)의 비율
    float widthRatio;   // 요소의 너비 비율 (0.0 ~ 1.0) → 화면 너비(screenWidth)의 비율
    float heightRatio;  // 요소의 높이 비율 (0.0 ~ 1.0) → 화면 높이(screenHeight)의 비율

    // 생성자: 비율 값을 받아 초기화
    UIElement(float x, float y, float width, float height) : xRatio(x), yRatio(y), widthRatio(width), heightRatio(height) {}

    // 현재 화면 크기(screenWidth, screenHeight)를 기준으로 UI 요소의 사각형 좌표(Rect)를 계산
    RECT GetRect(float screenWidth, float screenHeight)
    {
        RECT rect;

        // 좌측(xRatio 비율에 따라 화면 너비를 곱하여 계산)
        rect.left = LONG(xRatio * screenWidth);

        // 상단(yRatio 비율에 따라 화면 높이를 곱하여 계산)
        rect.top = LONG(yRatio * screenHeight);

        // 우측(좌측 좌표 + widthRatio 비율 * 화면 너비)
        rect.right = LONG(rect.left + (widthRatio * screenWidth));

        // 하단(상단 좌표 + heightRatio 비율 * 화면 높이)
        rect.bottom = LONG(rect.top + (heightRatio * screenHeight));

        return rect;
    }
};


