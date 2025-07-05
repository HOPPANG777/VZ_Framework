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
    // 타일셋 텍스처는 TileSetManager를 사용하고,
    // 타일맵은 TileMap 클래스를 unique_ptr으로 관리
    unique_ptr<TileMap> tmap;

private:
    // 맵 편집용 설정
    UINT spacing = 50;
    UINT width = (Values::GetWinWidth() * 2) / spacing;
    UINT height = (Values::GetWinHeight() * 2) / spacing;
    Vector3 tileSize = Vector3(64, 64, 0);

    // 타일셋 정보
    wstring tileSetPath = TexturePath + L"tile.png";
    UINT tileSetColumns = 8;
    UINT tileSetRows = 8;
    int selectedTileIndex = 0;

    // 파일명 (저장/불러오기)
    string currentMapFile = "map01.dat";

    // IMGUI에서 맵 배치 기준 위치
    ImVec2 mapOrigin = ImVec2(400, 50);

    bool isLoaded = false;

private:
    void CreateDefaultMap();

};
