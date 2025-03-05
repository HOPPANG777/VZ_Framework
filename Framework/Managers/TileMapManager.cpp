#include "Framework.h"
#include "TileMapManager.h"

TileMapManager::TileMapManager() {}

TileMapManager::~TileMapManager() {}

// 저장: 바이너리 형식으로 맵 크기와 각 타일의 인덱스를 저장합니다.
void TileMapManager::SaveMap(const vector<shared_ptr<Tile>>& tileMap, int rows, int cols, const string& fileName)
{
    // TilePath는 전역 혹은 상수로 정의되어 있는 타일 데이터 폴더 경로입니다.
    string fullPath = Path::Combine(TilePath, fileName);

    // FileStream을 쓰기 모드로 열기
    FileStream fs(fullPath, FILE_STREAM_WRITE);

    // 맵의 행과 열 저장
    fs.Write<int>(rows);
    fs.Write<int>(cols);

    // 각 타일의 tileIndex를 row-major 순서로 저장합니다.
    int total = rows * cols;
    for (int i = 0; i < total; ++i)
    {
        int tileIndex = tileMap[i]->GetTileIndex(); // Tile 클래스에 GetTileIndex()가 구현되어 있다고 가정
        fs.Write<int>(tileIndex);
    }
}

// 불러오기: 파일에서 맵 데이터를 읽어 Tile 객체들을 생성합니다.
vector<shared_ptr<Tile>> TileMapManager::LoadMap(const string& fileName, int& outRows, int& outCols, const Vector3& tileSize, const wstring& tileSetPath, UINT tileSetColumns, UINT tileSetRows)
{
    vector<shared_ptr<Tile>> tileMap;
    string fullPath = Path::Combine(TilePath, fileName);

    // FileStream을 읽기 모드로 열기
    FileStream fs(fullPath, FILE_STREAM_READ);

    // 맵의 행과 열 읽기
    outRows = fs.Read<int>();
    outCols = fs.Read<int>();

    int total = outRows * outCols;
    vector<int> tileIndices;
    for (int i = 0; i < total; ++i)
    {
        int index = fs.Read<int>();
        tileIndices.push_back(index);
    }

    // 타일 객체 생성 (row-major 순서)
    for (int i = 0; i < outRows; ++i)
    {
        for (int j = 0; j < outCols; ++j)
        {
            // 예: 각 타일의 중심 좌표를 계산 (좌측 상단 기준)
            Vector3 pos(j * tileSize.x + tileSize.x * 0.5f, i * tileSize.y + tileSize.y * 0.5f, 0);
            int tileIndex = tileIndices[i * outCols + j];

            auto tile = make_shared<Tile>
            (
                "Tile_" + to_string(i) + "_" + to_string(j),
                pos,
                tileSize,
                0.0f,
                tileSetPath,    // 타일셋 이미지 경로
                tileSetColumns, // 타일셋의 가로 타일 수
                tileSetRows,    // 타일셋의 세로 타일 수
                tileIndex       // 저장된 타일 인덱스
            );
            tileMap.push_back(tile);
        }
    }
    return tileMap;
}
