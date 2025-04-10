#include "stdafx.h"
#include "ParallaxBackground.h"

ParallaxBackground::ParallaxBackground(const string& compName, int layerCount)
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

void ParallaxBackground::SetLayer(int index, shared_ptr<Object> layer, shared_ptr<Object> layerDup)
{
    if (index < 0 || index >= layerCount) return;
    bgLayer[index] = layer;
    bgLayerDup[index] = layerDup;
    if (layer) bgOrigin[index] = layer->GetPosition();
    if (layerDup) bgOriginDup[index] = layerDup->GetPosition();
}

void ParallaxBackground::Update()
{
    // 카메라 위치를 기준으로 패럴럭스 효과 적용
    Vector3 camPos = CAMERA->GetPosition();

    // 레이어별 패럴럭스 팩터 (예: 선형 보간으로 0.2 ~ 1.0)
    for (int i = 0; i < layerCount; i++)
    {
        if (bgLayer[i] && bgLayerDup[i])
        {
            // 예시: i=0일 때 0.2, i=layerCount-1일 때 1.0
            float factor = 0.2f + (i * (0.8f / (layerCount - 1)));
            float offsetX = camPos.x * factor;

            // 새로운 배경 위치 계산 (원본 및 복제)
            Vector3 newBgPos = bgOrigin[i] + Vector3(offsetX, 0, 0);
            Vector3 newBgDupPos = bgOriginDup[i] + Vector3(offsetX, 0, 0);

            bgLayer[i]->SetPosition(newBgPos);
            bgLayerDup[i]->SetPosition(newBgDupPos);

            // 무한 스크롤 처리: 배경 너비 (여기서는 WIN_DEFAULT_WIDTH로 가정)
            float bgWidth = WIN_DEFAULT_WIDTH * 1.2f;

            //// 만약 원본 배경이 완전히 왼쪽으로 벗어났다면, 복제 배경의 오른쪽에 재배치
            //if (newBgPos.x + bgWidth * 0.5f < camPos.x)
            //{
            //    bgLayer[i]->SetPosition(bgLayerDup[i]->GetPosition() + Vector3(bgWidth, 0, 0));
            //    std::swap(bgLayer[i], bgLayerDup[i]);
            //}
            //// 반대로 원본 배경이 완전히 오른쪽으로 벗어났다면, 복제 배경의 왼쪽에 재배치
            //else if (newBgPos.x > camPos.x + WIN_DEFAULT_WIDTH - bgWidth * 0.5f)
            //{
            //    bgLayer[i]->SetPosition(bgLayerDup[i]->GetPosition() - Vector3(bgWidth, 0, 0));
            //    std::swap(bgLayer[i], bgLayerDup[i]);
            //}

            while (newBgPos.x + bgWidth * 0.5f < camPos.x)
            {
                // 배경이 화면 왼쪽 밖으로 벗어났다면, 오른쪽으로 bgWidth만큼 이동
                bgLayer[i]->SetPosition(bgLayerDup[i]->GetPosition() + Vector3(bgWidth, 0, 0));
                std::swap(bgLayer[i], bgLayerDup[i]);

                // 다시 newBgPos를 갱신해서 다음 루프에서 또 검사
                newBgPos = bgLayer[i]->GetPosition();
            }

            // 반대쪽도 while 루프로
            while (newBgPos.x > camPos.x + (WIN_DEFAULT_WIDTH * 1.2f) - bgWidth * 0.5f)
            {
                // 배경이 화면 오른쪽 밖으로 벗어났다면, 왼쪽으로 bgWidth만큼 이동
                bgLayer[i]->SetPosition(bgLayerDup[i]->GetPosition() - Vector3(bgWidth, 0, 0));
                std::swap(bgLayer[i], bgLayerDup[i]);

                newBgPos = bgLayer[i]->GetPosition();
            }
        }
    }
}

void ParallaxBackground::Render()
{
    // 각 레이어의 배경을 그립니다.
    for (int i = layerCount - 1; i >= 0; i--)
    {
        if (bgLayer[i]) bgLayer[i]->Render();
        if (bgLayerDup[i]) bgLayerDup[i]->Render();
    }
}
