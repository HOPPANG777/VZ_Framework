#include "stdafx.h"
#include "MapTools.h"

void MapTools::Awake()
{
    isLoaded = false;

    // 예시: 타일셋 매니저를 통해 타일셋을 변경 (Tilesets/Grass.png)
    TILESET->ChangeTileMap(TexturePath + L"Tilesets/StartSceneTile.png", 11, 7);
    //TILESET->ChangeTileMap(TexturePath + L"Tilesets/tile1.png", 21, 15);

    isLoaded = true;
}

void MapTools::Start()
{
    // 초기 맵 생성
    CreateDefaultMap();
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
    // 마우스가 ImGui에 의해 캡처되고 있다면 타일 입력 처리 건너뛰기
    if (ImGui::GetIO().WantCaptureMouse) return;

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
    // "Map Tools"라는 제목의 접이식 헤더를 추가합니다.
    if (ImGui::CollapsingHeader("Map Tools", ImGuiTreeNodeFlags_DefaultOpen))
    {
        if (ImGui::BeginTabBar("MapTabs"))
        {
            // 첫번째 탭: Map Editor
            if (ImGui::BeginTabItem("Map Editor"))
            {
                // spacing 입력 및 계산
                ImGui::InputInt("Spacing", reinterpret_cast<int*>(&spacing));
                //if (spacing <= 0) spacing = 50;

                width = (WIN_DEFAULT_WIDTH * 2) / spacing;
                height = (WIN_DEFAULT_HEIGHT * 2) / spacing;

                ImGui::Text("Calculated Map Width: %d", width);
                ImGui::Text("Calculated Map Height: %d", height);

                // 파일 이름 입력 필드
                static char fileNameBuffer[128] = "map01";
                ImGui::InputText("Map File Name", fileNameBuffer, IM_ARRAYSIZE(fileNameBuffer));

                if (ImGui::Button("Create New Map"))
                {
                    CreateDefaultMap();
                }
                ImGui::SameLine();

                if (ImGui::Button("Save Map"))
                {
                    // TilePath는 전역 또는 클래스 멤버로 설정되어 있다고 가정합니다.
                    string fullPath = string(TilePath.begin(), TilePath.end()) + fileNameBuffer + ".data";
                    string fulltextPath = string(TilePath.begin(), TilePath.end()) + fileNameBuffer + ".txt";
                    if (tmap) tmap->Save(fullPath);
                    if (tmap) tmap->Save(fulltextPath);
                }
                ImGui::SameLine();

                if (ImGui::Button("Load Map"))
                {
                    // 새 타일맵 생성 후 입력한 파일명으로 로드
                    tmap = make_unique<TileMap>(width, height, spacing);
                    string fullPath = string(TilePath.begin(), TilePath.end()) + fileNameBuffer + ".data";
                    tmap->Load(fullPath);
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
}

void MapTools::CreateDefaultMap()
{
    // 새로운 타일맵 생성 (mapWidth x mapHeight, spacing 사용)
    tmap = make_unique<TileMap>(width, height, spacing, true);
}