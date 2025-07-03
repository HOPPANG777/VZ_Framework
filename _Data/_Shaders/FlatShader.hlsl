// FlatShading.hlsl

// 구조체: 입력 정점 데이터
struct VS_INPUT
{
    float3 pos : POSITION; // 정점 위치
    float3 normal : NORMAL; // 정점 법선
};

// 구조체: 정점 셰이더 출력 데이터
struct VS_OUTPUT
{
    float4 posH : SV_POSITION; // 변환된 위치
    float3 normal : NORMAL; // 폴리곤 법선
};

// 월드, 뷰, 프로젝션 행렬
cbuffer MatrixBuffer : register(b0)
{
    matrix World;
    matrix View;
    matrix Projection;
}

// 광원 방향, 색상
cbuffer LightBuffer : register(b1)
{
    float3 LightDir;
    float3 LightColor;
    float3 ObjectColor;
}

// Vertex Shader
VS_OUTPUT VS_Main(VS_INPUT input)
{
    VS_OUTPUT output;
    float4 worldPos = mul(float4(input.pos, 1.0), World);
    output.posH = mul(mul(worldPos, View), Projection);

    // (플랫 셰이딩을 위해) 한 삼각형의 법선을 셰이더에서 받아왔다고 가정
    output.normal = normalize(mul(input.normal, (float3x3) World));
    return output;
}

// Pixel Shader: 폴리곤마다 하나의 색상만 적용 (Flat Shading)
float4 PS_Main(VS_OUTPUT input) : SV_TARGET
{
    // 광원 방향과 법선의 내적 → 밝기 계산
    float NdotL = saturate(dot(normalize(input.normal), -normalize(LightDir)));

    // 단일 색상만 적용 (Flat)
    float3 color = ObjectColor * LightColor * NdotL;
    return float4(color, 1.0);
}
