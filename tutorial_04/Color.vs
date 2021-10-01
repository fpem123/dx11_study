/*
 * Color.vs : ù ��° ���̴� ���α׷�
 * ���̴��� ���� ���� �������� �����ϴ� ���� ���α׷�
 * HLSL�� �ۼ���
 * �ܼ��ϸ鼭 �����ִ� �ﰢ���� �׸�
 * �ۼ��� : 09.30.21
 * ������ ������Ʈ : 09.30.21
 */

/////////////////
// Globals
/////////////////
// C++�� �����
// �������� cbuffer ���� ��ü Ÿ�Կ� �ְԵ�
// ���̴��� ȿ������ ����, gpu�� ��� ���۵��� �����ϴ� ���͵� ���õ�
// 3���� ��� ���, �� �����Ӹ��� ���ÿ� ������Ʈ �ϱ� ������ ���� ���ۿ�

cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};


/////////////////
// Type defs, ����� ���� Ÿ��
/////////////////
// HLSL���� �����Ѵ� float4 Ÿ�� ����Ͽ� ������ ���
// xyzw ��ġ ����, rgba ���� Ÿ��
// POSITION, COLOR, SV_POSITION�� gpu�� �� ������ ����� ���̶� ����
// ������ ������, ���� �� �ȼ� ���̴��� �ǹ̰� �ٸ��� ������ �ٸ� ����ü��
// COLOR : ��� ��������,
// POSIOTION : ���� ���̴�����
// SV_POSITION : �ȼ� ���̴����� ����
// ���� Ÿ���� �� ����ϰ� �ʹٸ� ���� �̸� ���� ���� ������

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
// ���� ���̴�
/////////////////
// ���� ������ �����͸� ó���ϱ����� GPU�� ȣ��
// ���� ������ ��� ������ ���� ȣ���
// �Է� VertexInputType : ���̴��� �ҽ����� ������ �ڷᱸ���� �� ������ �¾ƾ���
// ���� ���̴��� ó�� ����� �ȼ� ���̴��� ������
// PixelInputType ������ ���

PixelInputType ColorVertexShader(VertexInputType input)
{
	PixelInputType output;

	// �ùٸ��� ��� ������ �ϱ� ���� position ���͸�
	// w ���� �ִ� 4 ������ �ִ� ������ ���
	input.position.w = 1.0f;

	// ������ ��ġ�� ����, ��, ���� ������ ���� ���� 
	// 3D ������ ������ 2D ȭ���� ��ġ�� ����
	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	// �ȼ� ���̴����� ����ϱ� ���� �Է� ������ ����
	output.color = input.color;

	return output;
};