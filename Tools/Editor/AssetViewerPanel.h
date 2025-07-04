#pragma once

#include "IEditorPanel.h"
#include <memory>

// 에셋 뷰어 패널로 각종 리소스를 미리볼 수 있습니다.
class AssetViewerPanel : public IEditorPanel
{
public:
    void Init() override;
    void Update() override;
    void Render() override;
    void ImGUI() override;
};

