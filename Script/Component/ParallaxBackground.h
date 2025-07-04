#pragma once

#include "Components/Component.h"
//#include "Objects/BaseObject/GameObject.h"
#include "Utilities/DirectHelper.h"
#include "Systems/Camera.h"

class ParallaxBackground : public Component
{
    static const int MAX_LAYERS = 10; // 최대 레이어 개수

public:
    // 생성자: layerCount는 실제 사용하는 레이어 수 (기본 5)
    ParallaxBackground(const std::string& compName = "ParallaxBackground", int layerCount = 5);
    virtual ~ParallaxBackground() override = default;

public:
    // Component 인터페이스 구현
    virtual void Update() override;
    virtual void Render() override;

public:
    // 배경 오브젝트와 복제 오브젝트를 세팅하는 함수 (외부에서 호출)
    void SetLayer(int index, GameObject* layer, GameObject* layerDup);

private:
    // 실제 사용하는 레이어 수
    int layerCount = 5;

    // 각 레이어의 배경 오브젝트와 복제본 (배경 오브젝트는 TextureRect 등의 객체라고 가정)
    GameObject* bgLayer[MAX_LAYERS];
    GameObject* bgLayerDup[MAX_LAYERS];

    // 각 배경의 초기(원본) 위치 (월드 좌표)
    Vector3 bgOrigin[MAX_LAYERS];
    Vector3 bgOriginDup[MAX_LAYERS];

};

