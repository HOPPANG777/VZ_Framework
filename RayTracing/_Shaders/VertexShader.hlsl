/*
파이프 라인 : 파이프 라인의 생김새 처럼
한 파이프씩 거쳐 가며 현재의 출력이 다음의 입력이 되는 구조.

렌더링 파이프 라인 : 파이프라인을 거쳐 최종 렌더링 결과를 도출하는 구조

현재 렌더링 파이프 라인 중 VertexShader, PixelShader만 구현하며,
나머지 여러 쉐이더들도 있지만 기본적으로 알아서 처리해줌.
*/

struct VSInput
{
    float4 position : POSITION;
    float2 uv : TEXCOORD;
};

struct VSOutput
{
    float4 position : SV_PoSITION;
    float2 uv : TEXCOORD;
};

// vs의 진입점 (현재 인풋 -> 다음 출력)
VSOutput main(VSInput vsInput)
{
    VSOutput vsOutput;
    
    vsOutput.position = vsInput.position;
    vsOutput.uv = vsInput.uv;
    
	return vsOutput;
}