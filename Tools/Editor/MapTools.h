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
    // 추가: 타일셋 텍스처의 Shader Resource View
    ComPtr<ID3D11ShaderResourceView> tileSetSRV = nullptr;

    // 타일맵 데이터: row-major 순서로 저장
    vector<shared_ptr<Tile>> tileMap;
    int rows = 10;
    int cols = 10;

    // 편집용 기본 파라미터
    Vector3 tileSize = Vector3(64, 64, 0);
    wstring tileSetPath = TexturePath + L"TileSet.png";
    UINT tileSetColumns = 8;
    UINT tileSetRows = 8;

    ImVec2 mapOrigin = ImVec2(50, 100);

    // 파일명 (TilePath와 결합하여 사용)
    string currentMapFile = "map01.dat";

    // 편집 인터페이스에서 선택한 타일 인덱스 (예: 타일 변경에 사용)
    int selectedTileIndex = 0;

	bool isLoaded = false;


private:
    // 기본 맵 생성 함수
    void CreateDefaultMap();

};