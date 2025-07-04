#pragma once

#include "IEditorPanel.h"
#include "Objects/TileObject/TileMap.h"
#include "Utilities/Gui.h"

class MapEditorPanel : public IEditorPanel
{
public:
    ~MapEditorPanel();
    void Init() override;
    void Update() override;
    void Render() override;
    void ImGUI() override;

private:
    std::unique_ptr<TileMap> tmap;

    UINT spacing = 50;
    UINT width = (Values::GetWinWidth() * 2) / spacing;
    UINT height = (Values::GetWinHeight() * 2) / spacing;
    Vector3 tileSize = Vector3(64, 64, 0);

    std::wstring tileSetPath = TexturePath + L"tile.png";
    UINT tileSetColumns = 8;
    UINT tileSetRows = 8;
    int selectedTileIndex = 0;

    std::string currentMapFile = "map01.dat";

    ImVec2 mapOrigin = ImVec2(400, 50);

    // 맵 편집 시 사용될 카메라
    Camera* previewCamera = nullptr;
private:
    void CreateDefaultMap();
};

