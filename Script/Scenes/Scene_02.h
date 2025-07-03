#pragma once

#include "Interfaces/IScene.h"
#include "Objects/TextureObject/TextureRect.h"
#include "Objects/ColorObject/ColorRect.h"
#include "Managers/SceneManager.h"

class Scene_02 : public Scene
{
public:
    // Scene을(를) 통해 상속됨
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
    float timer = 0.0f;
    bool isDone = false;

    std::vector<std::shared_ptr<Object>> objects;

    //std::vector<std::shared_ptr<UIElement>> UIs;

    std::shared_ptr<Object> splash;

    bool isLoaded = false;

};

