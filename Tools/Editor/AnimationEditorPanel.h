#pragma once

#include "IEditorPanel.h"
#include <memory>

// 애니메이션 트랙을 편집하는 패널입니다.
class AnimationEditorPanel : public IEditorPanel
{
public:
    void Init() override;
    void Update() override;
    void Render() override;
    void ImGUI() override;
};

