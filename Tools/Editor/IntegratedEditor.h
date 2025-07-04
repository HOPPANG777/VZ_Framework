#pragma once

#include "Interfaces/IScene.h"
#include "IEditorPanel.h"
#include <memory>
#include <vector>

// 여러 편집 패널을 묶어 제공하는 통합 에디터 씬입니다.
class IntegratedEditor : public Scene
{
public:
    void Awake() override;
    void Start() override;
    void Destroy() override;

    void FixedUpdate() override;
    void Update() override;
    void LateUpdate() override;

    void Render() override;
    void PostRender() override;

    void ImGUI() override;

    bool IsLoaded() const override { return isLoaded; }

private:
    // 각각의 패널을 unique_ptr로 보관
    std::vector<std::unique_ptr<IEditorPanel>> panels;
    bool isLoaded = false;
};

