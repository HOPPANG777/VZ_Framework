#include "stdafx.h"
#include "MapTools.h"

void MapTools::Awake()
{
    isLoaded = false;

    // 초기 맵 생성
    CreateDefaultMap();

    // TileSet 싱글톤에서 필요한 정보를 바꿀 수도 있음
    // 예: TileSet::Get()->ChangeTileMap(tileSetPath, tileSetColumns, tileSetRows);

    TILESET->ChangeTileMap(TexturePath + L"Tilesets/Grass.png", 11, 7);

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
    // 마우스 클릭으로 맵에 타일 배치
    if (ImGui::IsMouseClicked(0) && !ImGui::GetIO().WantCaptureMouse)
    {
        ImVec2 mousePos = ImGui::GetMousePos();
        float offsetX = mousePos.x - mapOrigin.x;
        float offsetY = mousePos.y - mapOrigin.y;
        int gridX = (int)(offsetX / tileSize.x);
        int gridY = (int)(offsetY / tileSize.y);
        int index = gridY * cols + gridX;

        if (index >= 0 && index < (int)tileMap.size())
        {
            // 선택된 인덱스로 타일 교체
            tileMap[index]->SetTileIndex(selectedTileIndex);
        }
    }

    // 각 타일 업데이트
    for (auto& tile : tileMap)
    {
        if (tile) tile->Update();
    }
}

void MapTools::LateUpdate()
{

}

void MapTools::Render()
{
    // 1) 배경(맵) 그리기
    // Begin() ~ End() 사이에서 각 Tile의 Render() 호출
    for (auto& tile : tileMap)
    {
        if (!tile) continue;
        tile->Render();
    }

    // 2) IMGUI를 통한 UI는 ImGUI()에서 처리
    
}

void MapTools::PostRender()
{

}

void MapTools::ImGUI()
{
    // 타일셋 팔레트 창 (왼쪽 or 상단에 배치)
    {
        // 원하는 위치/크기 설정
        ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Once);
        ImGui::SetNextWindowSize(ImVec2(400, 600), ImGuiCond_Once);

        ImGui::Begin("TileSet");
        // TileSet 싱글톤 GUI (타일셋 이미지 표시)
        TILESET->ImGUI();

        // 현재 선택된 타일 인덱스 표시
        ImGui::Text("Selected Tile Index: %d", selectedTileIndex);

        // 맵 사이즈, 새 맵 생성
        ImGui::InputInt("Rows", &rows);
        ImGui::InputInt("Cols", &cols);
        if (ImGui::Button("Create New Map"))
        {
            CreateDefaultMap();
        }

        // 저장 / 불러오기
        if (ImGui::Button("Save Map"))
        {
            TILEMAP->SaveMap(tileMap, rows, cols, currentMapFile);
        }
        ImGui::SameLine();
        if (ImGui::Button("Load Map"))
        {
            tileMap = TILEMAP->LoadMap(currentMapFile, rows, cols, tileSize, tileSetPath, tileSetColumns, tileSetRows);
        }

        ImGui::End();
    }

    // 필요하다면, 타일맵 미리보기를 IMGUI 위젯으로도 표시 가능
    // 하지만 보통은 DirectX Render()에서 배경으로 그립니다.
}

void MapTools::CreateDefaultMap()
{
    tileMap.clear();
    tileMap.reserve(rows * cols);

    for (int y = 0; y < rows; ++y)
    {
        for (int x = 0; x < cols; ++x)
        {
            // 타일의 중심 좌표 계산 (mapOrigin 기준)
            Vector3 pos(mapOrigin.x + x * tileSize.x + tileSize.x * 0.5f, mapOrigin.y + y * tileSize.y + tileSize.y * 0.5f, 0.0f);

            // 타일셋 모드 사용 (TileSetPath, columns, rows, 인덱스=0)
            auto tile = make_shared<Tile>
            (
                "Tile_" + to_string(y) + "_" + to_string(x),
                pos,
                tileSize,
                0.0f,
                tileSetPath,
                tileSetColumns,
                tileSetRows,
                0 // 초기 인덱스
            );
            tileMap.push_back(tile);
        }
    }
}