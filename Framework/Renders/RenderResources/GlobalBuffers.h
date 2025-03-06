#pragma once

class WorldBuffer : public ConstantBuffer
{
	// 16바이트 단위로 데이터 패딩을 해줘야 한다.
	struct WorldStruct
	{
		Matrix world;
	};

	WorldStruct data;

public:
	WorldBuffer() : ConstantBuffer(&data, sizeof(data)) {}

	void SetWorld(const Matrix& world)
	{
		data.world = XMMatrixTranspose(world);
	}
};

class ViewProjectionBuffer : public ConstantBuffer
{
	struct ViewProjStruct
	{
		Matrix view;
		Matrix proj;
	};

	ViewProjStruct data;

public:
	ViewProjectionBuffer() : ConstantBuffer(&data, sizeof(data)) {}

	void SetView(const Matrix& view)
	{
		data.view = XMMatrixTranspose(view);
	}

	void SetProjection(const Matrix& proj)
	{
		data.proj = XMMatrixTranspose(proj);
	}
};

class ColorBuffer : public ConstantBuffer
{
	struct ColorStruct
	{
		Color color;
	};

	ColorStruct data;

public:
	ColorBuffer() : ConstantBuffer(&data, sizeof(data))
	{
		// 기본 변환 색상은 흰색으로 설정 (즉, 색상 변화 없음)
		data.color = Color(1.0f, 1.0f, 1.0f, 1.0f);
	}

	// 색상 변환 값 설정 (예: 텍스처 색상에 곱해질 값)
	void SetColor(const Color& color)
	{
		data.color = color;
	}

	// 현재 설정된 색상 변환 값을 반환 (읽기 전용)
	Color GetColor() const
	{
		return data.color;
	}
};

class IndexNumBuffer : public ConstantBuffer
{
	struct IndexStruct
	{
		UINT index;
		Vector3 dummy;
	};

	IndexStruct data;

public:
	IndexNumBuffer() : ConstantBuffer(&data, sizeof(data))
	{
		data.index = 0;
	}

	void SetIndex(UINT index) 
	{
		data.index = index; 
	}

};



