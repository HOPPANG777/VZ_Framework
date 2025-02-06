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
    // 비율 기반 위치 및 크기
    float xRatio;       // 요소의 왼쪽 위치 비율 (0.0 ~ 1.0)
    float yRatio;       // 요소의 위쪽 위치 비율 (0.0 ~ 1.0)
    float widthRatio;   // 요소의 너비 비율 (0.0 ~ 1.0)
    float heightRatio;  // 요소의 높이 비율 (0.0 ~ 1.0)

    // 픽셀 단위 오프셋
    int xOffset;        // 추가적으로 조정할 픽셀 단위 X 오프셋
    int yOffset;        // 추가적으로 조정할 픽셀 단위 Y 오프셋
    int widthOffset;    // 추가적으로 조정할 픽셀 단위 너비 오프셋
    int heightOffset;   // 추가적으로 조정할 픽셀 단위 높이 오프셋

    // 생성자
    UIElement(float x, float y, float width, float height, int xOff = 0, int yOff = 0, int wOff = 0, int hOff = 0)
        : xRatio(x), yRatio(y), widthRatio(width), heightRatio(height),
        xOffset(xOff), yOffset(yOff), widthOffset(wOff), heightOffset(hOff) 
    {
        //relativeX = x / float(baseScreenWidth);
        //relativeY = y / float(baseScreenHeight);
        //relativeWidth = width / float(baseScreenWidth);
        //relativeHeight = height / float(baseScreenHeight);
    }

    // 현재 화면 크기(screenWidth, screenHeight)를 기준으로 UI 요소의 사각형 좌표(Rect)를 계산
    RECT GetRect(float screenWidth, float screenHeight)
    {
        RECT rect;

        // 좌측 (비율 기반 위치 + 픽셀 오프셋)
        rect.left = LONG(xRatio * screenWidth) + xOffset;

        // 상단 (비율 기반 위치 + 픽셀 오프셋)
        rect.top = LONG(yRatio * screenHeight) + yOffset;

        // 우측 (좌측 좌표 + 비율 기반 너비 + 픽셀 오프셋)
        rect.right = LONG(rect.left + (widthRatio * screenWidth) + widthOffset);

        // 하단 (상단 좌표 + 비율 기반 높이 + 픽셀 오프셋)
        rect.bottom = LONG(rect.top + (heightRatio * screenHeight) + heightOffset);

        return rect;
    }

    // 위치 및 크기 설정 함수
    void SetPosition(float x, float y, int xOff = 0, int yOff = 0)
    {
        xRatio = x;
        yRatio = y;
        xOffset = xOff;
        yOffset = yOff;
    }

    void SetSize(float width, float height, int wOff = 0, int hOff = 0)
    {
        widthRatio = width;
        heightRatio = height;
        widthOffset = wOff;
        heightOffset = hOff;
    }
};


