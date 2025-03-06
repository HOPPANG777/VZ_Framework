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
    if (ImGui::BeginTabBar("MapTabs"))
    {
        // 첫번째 탭: Map Editor
        if (ImGui::BeginTabItem("Map Editor"))
        {
            // spacing만 입력받고, width와 height는 계산
            ImGui::InputInt("Spacing", reinterpret_cast<int*>(&spacing));
            // spacing이 0이 아닌지 체크
            if (spacing <= 0)
                spacing = 1;

            // WIN_DEFAULT_WIDTH, WIN_DEFAULT_HEIGHT는 전역 상수로 정의되어 있음
            width = (WIN_DEFAULT_WIDTH * 2) / spacing;
            height = (WIN_DEFAULT_HEIGHT * 2) / spacing;

            ImGui::Text("Calculated Map Width: %d", width);
            ImGui::Text("Calculated Map Height: %d", height);

            if (ImGui::Button("Create New Map"))
            {
                CreateDefaultMap();
            }
            if (ImGui::Button("Save Map"))
            {
                if (tmap) tmap->Save(currentMapFile);
            }

            ImGui::SameLine();
            if (ImGui::Button("Load Map"))
            {
                // 새 타일맵 생성 후 파일로부터 로드
                tmap = make_unique<TileMap>(width, height, spacing);
                tmap->Load(currentMapFile);
            }
            ImGui::EndTabItem();
        }

        // 두번째 탭: TileMap ImGui (타일맵 자체의 UI)
        if (ImGui::BeginTabItem("TileMap ImGui"))
        {
            if (tmap) tmap->ImGUI();
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }
}

void MapTools::CreateDefaultMap()
{
    // 새로운 타일맵 생성 (mapWidth x mapHeight, spacing 사용)
    tmap = make_unique<TileMap>(width, height, spacing);
}