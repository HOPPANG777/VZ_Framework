#pragma once

class Scene_03 : public Scene
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


};

