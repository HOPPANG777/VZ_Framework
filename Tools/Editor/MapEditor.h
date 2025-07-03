#pragma once

#include "Interfaces/IScene.h"
#include "Objects/TileObject/TileMap.h"
#include "Utilities/Gui.h"

class MapEditor : public Scene
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
    // 타일맵은 TileMap 클래스를 std::unique_ptr으로 관리
    std::unique_ptr<TileMap> tmap;

private:
    // 맵 편집용 설정
    UINT spacing = 50;
    UINT width = (WIN_DEFAULT_WIDTH * 2) / spacing;
    UINT height = (WIN_DEFAULT_HEIGHT * 2) / spacing;
    Vector3 tileSize = Vector3(64, 64, 0);

    // 타일셋 정보
    std::wstring tileSetPath = TexturePath + L"tile.png";
    UINT tileSetColumns = 8;
    UINT tileSetRows = 8;
    int selectedTileIndex = 0;

    // 파일명 (저장/불러오기)
    std::string currentMapFile = "map01.dat";

    // IMGUI에서 맵 배치 기준 위치
    ImVec2 mapOrigin = ImVec2(400, 50);

    bool isLoaded = false;

private:
    void CreateDefaultMap();

};
