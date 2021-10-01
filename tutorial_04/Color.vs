/*
 * Color.vs : 첫 번째 셰이더 프로그램
 * 셰이더는 실제 모델의 렌더링을 수행하는 작은 프로그램
 * HLSL로 작성됨
 * 단순하면서 색상있는 삼각형을 그림
 * 작성일 : 09.30.21
 * 마지막 업데이트 : 09.30.21
 */

/////////////////
// Globals
/////////////////
// C++와 비슷함
// 변수들은 cbuffer 버퍼 객체 타입에 넣게됨
// 셰이더의 효율적인 실행, gpu가 어떻게 버퍼들을 저장하는 지와도 관련됨
// 3개의 행렬 사용, 매 프레임마다 동시에 업데이트 하기 때문에 같은 버퍼에

cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};


/////////////////
// Type defs, 사용자 정의 타입
/////////////////
// HLSL에서 지원한느 float4 타입 사용하여 가독성 상승
// xyzw 위치 벡터, rgba 색상 타입
// POSITION, COLOR, SV_POSITION은 gpu에 이 변수를 사용할 것이라 전달
// 구조는 같지만, 정정 및 픽셀 셰이더의 의미가 다르기 때문에 다른 구조체로
// COLOR : 모두 가지지만,
// POSIOTION : 정점 셰이더에서
// SV_POSITION : 픽셸 셰이더에서 동작
// 같은 타입을 더 사용하고 싶다면 변수 이름 끝에 숫자 붙히기

struct VertexInputType
{
	float4 position : POSITION;
	float4 color : COLOR;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};


/////////////////
// 정점 셰이더
/////////////////
// 정점 버퍼의 데이터를 처리하기위해 GPU가 호출
// 전점 버퍼의 모든 정점에 대해 호출됨
// 입력 VertexInputType : 셰이더의 소스에서 정의한 자료구조와 그 구조가 맞아야함
// 정점 셰이더의 처리 결과는 픽셀 셰이더로 보내짐
// PixelInputType 형태의 출력

PixelInputType ColorVertexShader(VertexInputType input)
{
	PixelInputType output;

	// 올바르게 행렬 연산을 하기 위해 position 백터를
	// w 까지 있는 4 성분이 있는 것으로 사용
	input.position.w = 1.0f;

	// 정점의 위치를 월드, 뷰, 투영 순으로 곱셈 연산 
	// 3D 세계의 정점을 2D 화면의 위치로 지정
	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	// 픽셀 셰이더에서 사용하기 위해 입력 색성을 저장
	output.color = input.color;

	return output;
};