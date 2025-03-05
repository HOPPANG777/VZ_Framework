#include "stdafx.h"
#include "MapTools.h"

void MapTools::Awake()
{
    isLoaded = false;

    // 타일셋 텍스처 로드
    HRESULT hr = D3DX11CreateShaderResourceViewFromFile
    (
        DEVICE,
        tileSetPath.c_str(),
        nullptr,
        nullptr,
        tileSetSRV.GetAddressOf(),
        nullptr
    );
    CHECK(hr);

    CreateDefaultMap();
    
    isLoaded = true;
}

void MapTools::Start()
{

}

void MapTools::Destroy()
{
    tileMap.clear();
}

void MapTools::FixedUpdate()
{

}

void MapTools::Update()
{
    for (const auto& tile : tileMap) if (tile) tile->Update();
    
    // 맵 배치 로직: 마우스 클릭을 통해 타일을 교체
    if (ImGui::IsMouseClicked(0) && !ImGui::GetIO().WantCaptureMouse)
    {
        ImVec2 mousePos = ImGui::GetMousePos();
        int gridX = static_cast<int>((mousePos.x - mapOrigin.x) / tileSize.x);
        int gridY = static_cast<int>((mousePos.y - mapOrigin.y) / tileSize.y);
        int index = gridY * cols + gridX;
        if (index >= 0 && index < tileMap.size())
        {
            tileMap[index]->SetTileIndex(selectedTileIndex);
        }
    }
}

void MapTools::LateUpdate()
{

}

void MapTools::Render()
{
    for (const auto& tile : tileMap) if (tile) tile->Render();
    
}

void MapTools::PostRender()
{

}

void MapTools::ImGUI()
{
    ImGui::Begin("Map Editor");

    ImGui::InputInt("Rows", &rows);
    ImGui::InputInt("Columns", &cols);
    if (ImGui::Button("Create New Map"))
    {
        CreateDefaultMap();
    }

    // 타일셋 선택 UI
    ImGui::Text("Tile Palette");
    // 타일셋 이미지를 표시 (tileSetSRV는 미리 로드된 타일셋 SRV)
    ImGui::Image((ImTextureID)tileSetSRV.Get(), ImVec2(256, 256));
    if (ImGui::IsItemClicked())
    {
        ImVec2 imagePos = ImGui::GetItemRectMin();
        ImVec2 mousePos = ImGui::GetMousePos();
        float relX = mousePos.x - imagePos.x;
        float relY = mousePos.y - imagePos.y;
        float tileWidth = 256.0f / tileSetColumns;
        float tileHeight = 256.0f / tileSetRows;
        int col = static_cast<int>(relX / tileWidth);
        int row = static_cast<int>(relY / tileHeight);
        selectedTileIndex = row * tileSetColumns + col;
    }
    ImGui::Text("Selected Tile Index: %d", selectedTileIndex);

    if (ImGui::Button("Save Map"))
    {
        TileMapManager::Get()->SaveMap(tileMap, rows, cols, currentMapFile);
    }
    if (ImGui::Button("Load Map"))
    {
        tileMap = TileMapManager::Get()->LoadMap(currentMapFile, rows, cols, tileSize, tileSetPath, tileSetColumns, tileSetRows);
    }

    ImGui::End();
}

void MapTools::CreateDefaultMap()
{
    tileMap.clear();
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            // 각 타일의 중심 좌표 계산 (예: 좌측 상단 기준)
            Vector3 pos(j * tileSize.x + tileSize.x * 0.5f, i * tileSize.y + tileSize.y * 0.5f, 0);
            // 기본 타일 인덱스는 0으로 설정
            auto tile = make_shared<Tile>(
                "Tile_" + to_string(i) + "_" + to_string(j),
                pos,
                tileSize,
                0.0f,
                tileSetPath,
                tileSetColumns,
                tileSetRows,
                0 // 초기 타일 인덱스
            );
            tileMap.push_back(tile);
        }
    }
}