#include "stdafx.h"
#include "MapTools.h"

void MapTools::Awake()
{
    isLoaded = false;

    // 초기 맵 생성
    CreateDefaultMap();

    // 예시: 타일셋 매니저를 통해 타일셋을 변경 (Tilesets/Grass.png)
    TILESET->ChangeTileMap(TexturePath + L"Tilesets/Grass.png", 11, 7);

    isLoaded = true;
}

void MapTools::Start()
{
    // 추가 시작 로직이 필요하면 여기에 작성
}

void MapTools::Destroy()
{
    tmap.reset();
}

void MapTools::FixedUpdate()
{
    // 고정 업데이트 로직 (필요시)
}

void MapTools::Update()
{
    //// 마우스 클릭으로 타일 배치 처리 (IMGUI가 마우스 입력을 캡쳐하지 않을 때)
    //if (ImGui::IsMouseClicked(0) && !ImGui::GetIO().WantCaptureMouse)
    //{
    //    ImVec2 mousePos = ImGui::GetMousePos();
    //    float offsetX = mousePos.x - mapOrigin.x;
    //    float offsetY = mousePos.y - mapOrigin.y;
    //    // 그리드 좌표 계산 (타일 크기를 기준으로)
    //    int gridX = static_cast<int>(offsetX / tileSize.x);
    //    int gridY = static_cast<int>(offsetY / tileSize.y);

    //    // 타일맵 내에서 해당 좌표의 타일 가져오기
    //    if (tmap)
    //    {
    //        // 타일의 좌표는 mapOrigin 기준 오프셋을 사용하여 계산
    //        Tile* tile = tmap->GetTile(Vector3(offsetX, offsetY, 0));
    //        if (tile)
    //        {
    //            tile->SetTileIndex(selectedTileIndex);
    //        }
    //    }
    //}

    if (tmap) tmap->Update();
}

void MapTools::LateUpdate()
{
    // 후처리 업데이트 (필요시)
}

void MapTools::Render()
{
    if (tmap) tmap->Render();
}

void MapTools::PostRender()
{
    // 포스트 렌더링 (필요시)
}

void MapTools::ImGUI()
{
    ImGui::Begin("Map Editor");

    ImGui::InputInt("Rows", &rows);
    ImGui::InputInt("Cols", &cols);
    if (ImGui::Button("Create New Map"))
    {
        CreateDefaultMap();
    }
    if (ImGui::Button("Save Map"))
    {
        tmap->Save(currentMapFile);
    }
    ImGui::SameLine();
    if (ImGui::Button("Load Map"))
    {
        // 기존 tmap를 재생성하고 파일로부터 로드
        tmap = make_unique<TileMap>(cols, rows, (UINT)tileSize.x);
        tmap->Load(currentMapFile);
    }
    ImGui::End();
}

void MapTools::CreateDefaultMap()
{
    tmap = make_unique<TileMap>(cols, rows, (UINT)tileSize.x);
}