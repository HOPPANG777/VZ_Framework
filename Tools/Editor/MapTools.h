#pragma once

class MapTools : public Scene
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
    // 맵 편집용
    int rows = 10;
    int cols = 10;
    Vector3 tileSize = Vector3(64, 64, 0);

    // 타일맵(1차원 벡터, row-major)
    vector<shared_ptr<Tile>> tileMap;

    // 타일셋 정보
    wstring tileSetPath = TexturePath + L"tile.png";
    UINT tileSetColumns = 8;
    UINT tileSetRows = 8;
    int selectedTileIndex = 0;

    // 파일명
    string currentMapFile = "map01.dat";

    // IMGUI에서 맵을 배치할 기준 위치
    ImVec2 mapOrigin = ImVec2(400, 50);

    bool isLoaded = false;

private:
    void CreateDefaultMap();

};