#include "stdafx.h"
#include "SceneEditorPanel.h"

void SceneEditorPanel::Init()
{
    // 카메라는 싱글톤으로 제공되므로 인스턴스를 가져와 사용한다
    previewCamera = Camera::Get();
    previewCamera->Init();
}

void SceneEditorPanel::Update()
{
    // 카메라 등 필요한 업데이트 수행
    if (previewCamera)
        previewCamera->Update();
}

void SceneEditorPanel::Render()
{
    // 미리보기 화면 렌더링
    if (previewCamera)
        previewCamera->Render();
}

void SceneEditorPanel::ImGUI()
{
    ImGui::Begin("Scene Editor");
    ImGui::Text("씬을 실시간으로 편집합니다.");
    ImGui::End();
}

