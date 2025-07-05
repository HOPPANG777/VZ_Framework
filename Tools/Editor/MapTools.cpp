#include "stdafx.h"
#include "MapTools.h"

void MapTools::Awake()
{
    isLoaded = false;

    // 예시: 타일셋 매니저를 통해 타일셋을 변경 (Tilesets/Grass.png)
    //TILESET->ChangeTileMap(TexturePath + L"Tilesets/Tile1.png", 8, 8);
    //TILESET->ChangeTileMap(TexturePath + L"Tilesets/tile1.png", 21, 15);

    isLoaded = true;
}

void MapTools::Start()
{
    // 초기 맵 생성
    //CreateDefaultMap();
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
    // "Map Editor"라는 제목의 독립 창으로 생성 (멀티 뷰포트 사용 시, 창을 분리할 수 있음)
    ImGui::Begin("Map Editor");
    {
        // ────────────────
        // 맵 크기 설정 관련
        // ────────────────
        ImGui::InputInt("Spacing", reinterpret_cast<int*>(&spacing));
        // spacing 값을 기준으로 맵의 가로, 세로 타일 개수 계산
        width = (Values::GetWinWidth() * 2) / spacing;
        height = (Values::GetWinHeight() * 2) / spacing;
        ImGui::Text("Calculated Map Width: %d", width);
        ImGui::Text("Calculated Map Height: %d", height);

        // ────────────────
        // 파일 이름 입력 (저장/불러오기)
        // ────────────────
        static char fileNameBuffer[128] = "map01";
        ImGui::InputText("Map File Name", fileNameBuffer, IM_ARRAYSIZE(fileNameBuffer));

        // ────────────────
        // 새 맵 생성, 저장, 불러오기, 맵 크기 확장 버튼
        // ────────────────
        if (ImGui::Button("Create New Map"))
        {
            CreateDefaultMap();
        }
        ImGui::SameLine();
        if (ImGui::Button("Save Map"))
        {
            string fullPath = string(TilePath.begin(), TilePath.end()) + fileNameBuffer + ".data";
            string fulltextPath = string(TilePath.begin(), TilePath.end()) + fileNameBuffer + ".txt";
            if (tmap) 
            {
                tmap->Save(fullPath);
                tmap->Save(fulltextPath);
            }
        }
        ImGui::SameLine();
        if (ImGui::Button("Load Map"))
        {
            // 새 타일맵 생성 후 파일명으로 로드
            tmap = make_unique<TileMap>(width, height, spacing, true);
            string fullPath = string(TilePath.begin(), TilePath.end()) + fileNameBuffer + ".data";
            tmap->Load(fullPath);
        }
        //ImGui::SameLine();
        static int newMapWidth = width;   // 기존 width 값을 초기값으로 사용
        static int newMapHeight = height; // 기존 height 값을 초기값으로 사용

        ImGui::InputInt("New Map Width", &newMapWidth);
        ImGui::InputInt("New Map Height", &newMapHeight);

        if (ImGui::Button("Resize Map"))
        {
            if (tmap) tmap->ResizeMap(newMapWidth, newMapHeight);
        }

        // ────────────────
        // Tileset 설정
        // ────────────────
        if (ImGui::CollapsingHeader("Tileset Settings", ImGuiTreeNodeFlags_DefaultOpen))
        {
            // 타일셋 이름만 입력받도록 변경 (예: "MyTileSet.png")
            static char tileSetNameBuffer[256] = "MyTileSet.png";
            ImGui::InputText("Tileset Name", tileSetNameBuffer, IM_ARRAYSIZE(tileSetNameBuffer));

            static int tileSetColumns = TILESET->GetXCount();
            static int tileSetRows = TILESET->GetYCount();
            ImGui::InputInt("Tileset Columns", &tileSetColumns);
            ImGui::InputInt("Tileset Rows", &tileSetRows);

            if (ImGui::Button("Apply Tileset"))
            {
                // 입력받은 타일셋 이름을 wstring으로 변환
                wstring tileSetName = String::ToWstring(string(tileSetNameBuffer));
                // 기본 경로 + "Tilesets/" + 사용자 입력 타일셋 이름
                wstring newTilePath = TexturePath + L"Tilesets/" + tileSetName + L".png";
                TILESET->ChangeTileMap(newTilePath, tileSetColumns, tileSetRows);
            }
        }

        // ────────────────
        // 타일맵 자체의 ImGUI (타일 팔레트 등)
        // ────────────────
        if (tmap) tmap->ImGUI();
    }
    ImGui::End();
}

void MapTools::CreateDefaultMap()
{
    // 새로운 타일맵 생성 (mapWidth x mapHeight, spacing 사용)
    tmap = make_unique<TileMap>(width, height, spacing, true);
}