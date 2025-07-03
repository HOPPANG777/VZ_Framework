#include "stdafx.h"
#include "IntegratedEditor.h"
#include "SceneEditorPanel.h"
#include "AnimationEditorPanel.h"
#include "AssetViewerPanel.h"

void IntegratedEditor::Awake()
{
	isLoaded = false;

    // 패널들을 생성하여 벡터에 추가합니다.
    panels.emplace_back(std::make_unique<SceneEditorPanel>());
    panels.emplace_back(std::make_unique<AnimationEditorPanel>());
    panels.emplace_back(std::make_unique<AssetViewerPanel>());

    for (auto& panel : panels) panel->Init();

    isLoaded = true;
}

void IntegratedEditor::Start()
{
}

void IntegratedEditor::Destroy()
{
    panels.clear();
}

void IntegratedEditor::FixedUpdate()
{
}

void IntegratedEditor::Update()
{
    for (auto& panel : panels) panel->Update();
}

void IntegratedEditor::LateUpdate()
{
}

void IntegratedEditor::Render()
{
    for (auto& panel : panels) panel->Render();
}

void IntegratedEditor::PostRender()
{
}

void IntegratedEditor::ImGUI()
{
    for (auto& panel : panels) panel->ImGUI();
}

