#pragma once

class Scene_01 : public Scene
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
	vector<shared_ptr<Object>> objects;

	shared_ptr<Object> startButton;

	bool isLoaded = false;
};

