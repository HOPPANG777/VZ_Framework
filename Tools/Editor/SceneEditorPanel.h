#pragma once

#include "IEditorPanel.h"
#include "Systems/Camera.h"
#include <memory>

// 씬 편집을 담당하는 패널입니다.
// 실시간 미리보기를 제공하여 WYSIWYG 방식으로 동작합니다.
class SceneEditorPanel : public IEditorPanel
{
public:
    void Init() override;
    void Update() override;
    void Render() override;
    void ImGUI() override;

private:
    // 미리보기용 카메라
    std::shared_ptr<Camera> previewCamera;
};

