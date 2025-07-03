#include "stdafx.h"
#include "ParallaxBackground.h"

ParallaxBackground::ParallaxBackground(const std::string& compName, int layerCount)
    : Component(compName), layerCount(layerCount)
{
    // 초기화
    for (int i = 0; i < MAX_LAYERS; i++)
    {
        bgLayer[i] = nullptr;
        bgLayerDup[i] = nullptr;
        bgOrigin[i] = Values::ZeroVec3;
        bgOriginDup[i] = Values::ZeroVec3;
    }
}

void ParallaxBackground::SetLayer(int index, Object* layer, Object* layerDup)
{
    if (index < 0 || index >= layerCount) return;
    bgLayer[index] = layer;
    bgLayerDup[index] = layerDup;
    if (layer)    bgOrigin[index] = layer->GetPosition();
    if (layerDup) bgOriginDup[index] = layerDup->GetPosition();
}

void ParallaxBackground::Update()
{
    // 카메라 위치를 기준으로 패럴럭스 효과 적용
    Vector3 camPos = Camera::Get()->GetPosition();

    for (int i = 0; i < layerCount; i++)
    {
        if (bgLayer[i] && bgLayerDup[i])
        {
            // 레이어별 패럴럭스 팩터 (예: i=0일 때 0.2, i=layerCount-1일 때 1.0)
            float factor = 0.2f + (i * (0.8f / (layerCount - 1)));

            // 카메라 이동에 따른 X, Y축 오프셋 계산
            float offsetX = camPos.x * factor;
            float offsetY = camPos.y * factor;

            // 원본 및 복제 배경 위치 계산 (X, Y 모두 적용)
            Vector3 newBgPos = bgOrigin[i] + Vector3(offsetX, offsetY, 0);
            Vector3 newBgDupPos = bgOriginDup[i] + Vector3(offsetX, offsetY, 0);

            bgLayer[i]->SetPosition(newBgPos);
            bgLayerDup[i]->SetPosition(newBgDupPos);

            // 무한 X 스크롤 처리 (배경 너비는 Values::WinDefaultWidth * 1.2로 가정)
            float bgWidth = Values::WinDefaultWidth * 1.2f;

            // 왼쪽 경계 넘김: bgLayer가 화면 왼쪽 밖으로 나가면 오른쪽으로 배치
            while (newBgPos.x + bgWidth * 0.5f < camPos.x)
            {
                bgLayer[i]->SetPosition(bgLayerDup[i]->GetPosition() + Vector3(bgWidth, 0, 0));
                std::swap(bgLayer[i], bgLayerDup[i]);
                newBgPos = bgLayer[i]->GetPosition();
            }

            // 오른쪽 경계 넘김: bgLayer가 화면 오른쪽 밖으로 나가면 왼쪽으로 배치
            float rightBound = camPos.x + (Values::WinDefaultWidth * 1.2f) - bgWidth * 0.5f;

            while (newBgPos.x > rightBound)
            {
                bgLayer[i]->SetPosition(bgLayerDup[i]->GetPosition() - Vector3(bgWidth, 0, 0));
                std::swap(bgLayer[i], bgLayerDup[i]);
                newBgPos = bgLayer[i]->GetPosition();
            }
        }
    }
}

void ParallaxBackground::Render()
{
    // 각 레이어의 배경을 뒤에서부터 그립니다.
    for (int i = layerCount - 1; i >= 0; i--)
    {
        if (bgLayer[i])    bgLayer[i]->Render();
        if (bgLayerDup[i]) bgLayerDup[i]->Render();
    }
}