#pragma once

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

private:
	void Clock();

private:
	vector<shared_ptr<Object>> objects;

	shared_ptr<Object> sceneNameObject;

	shared_ptr<Texts> sceneName;

	shared_ptr<Object> hourHand;
	shared_ptr<Object> minuteHand;
	shared_ptr<Object> secondHand;

	bool isColor = false;
};

