/*
 * Colorshaderclass.cpp : GPU상에 존재하는 3D 모델들을 그리는데 사용하는
 * HLSL 셰이더를 호출하는 클래스
 * 작성일 : 10.01.21
 * 마지막 업데이트 : 10.01.21
 */

//////////////////
// includes
//////////////////
#include "Colorshaderclass.h"

ColorShaderClass::ColorShaderClass()
{
	// 초기화
	m_vertexShader = 0;
	m_pixelShader = 0;
	m_layout = 0;
	m_matrixBuffer = 0;
}

ColorShaderClass::ColorShaderClass(const ColorShaderClass& other)
{

}

ColorShaderClass::~ColorShaderClass()
{

}

bool ColorShaderClass::Initialize(ID3D11Device* device, HWND hwnd)
{
	bool result;

	// 정점 셰이더와 픽셀 셰이더를 초기화
	// HLSL 셰이더 파일인 color.vs, color.ps 을 넘겨줌
	result = InitializeShader(device, hwnd,
		L"./color.vs", L"./color.ps");
	if (!result)
	{
		return false;
	}

	return true;
}

void ColorShaderClass::Shutdown()
{
	// 정점 셰이더와 픽셀 셰이더 및 그와 관련된 것들 정리
	ShutdownShader();

	return;
}


bool ColorShaderClass::Render(ID3D11DeviceContext* deviceContext, int indexCount,
	XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix)
{
	bool result;

	// 렌더링에 사용할 셰이더 인자를 입력
	// 셰이더에서 사용될 인자들을 설정함
	result = SetShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix);
	if (!result)
	{
		return false;
	}

	// 셰이더를 이용해 준비된 버퍼를 그림
	// HLSL 셰이더를 이용한 녹색 삼각형
	RenderShader(deviceContext, indexCount);

	return true;
}

// 이번 튜토리얼에서 가장 중요한 함수
// 실제로 셰이더 파일을 불러오고 DirectX와 GPU에서 사용 가능하도록 하게함
// 레이아웃을 세팅, 어떻게 정점 버퍼의 데이터가 GPU에 사용되는지 볼 수 있음
// 레이아웃은 modelclass.h 및 color.vs 에 선언된  VertexType과 일치해야함
bool ColorShaderClass::InitializeShader(ID3D11Device* device, HWND hwnd,
	const WCHAR* vsFilename, const WCHAR* psFilename)
{
	HRESULT result;
	ID3D10Blob* errorMessage;
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
	unsigned int numElements;
	D3D11_BUFFER_DESC matrixBufferDesc;

	// 이 함수에서 사용되는 포인터 null로 설정
	errorMessage = 0;
	vertexShaderBuffer = 0;
	pixelShaderBuffer = 0;


	// 셰이더 프로그램을 버퍼로 컴파일
	// 세이더 파일 이름, 셰이더 이름, 셰이더 버전(다렉11에선 5.0), 컴파일 될 버퍼를 인자로
	// 컴파일 실패시 errorMessage 문자열에 출력, 다른 함수로 볼 수 있음
	// errorMessage 없이 실패하면 셰이더 파일 자체가 없음 -> 알 수 있도록 다이얼로그 생성


	// 정점 셰이더를 컴파일
	// D3DX11CompileFromFile -> D3DCompileFromFile
	result = D3DCompileFromFile(vsFilename, NULL, NULL, "ColorVertexShader",
		"vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &vertexShaderBuffer, &errorMessage);
	if (FAILED(result))
	{
		// 셰이더가 컴파일에 실패하면 에러 메시지를 기록
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, vsFilename);
		}
		// 에러메시지가 없음 == 셰이더 파일을 찾지 못함
		else
		{
			MessageBox(hwnd, vsFilename, L"Missing Shader FIle", MB_OK);
		}

		return false;
	}

	// 픽셀 셰이더를 컴파일
	result = D3DCompileFromFile(psFilename, NULL, NULL, "ColorPixelShader",
		"ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &pixelShaderBuffer, &errorMessage);
	if (FAILED(result))
	{
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, psFilename);
		}
		else
		{
			MessageBox(hwnd, psFilename, L"Missing Shader File", MB_OK);
		}

		return false;
	}

	
	// 컴파일 된 셰이더들로 셰이더 객체를 만듬
	// 여기서 나온 포인터를 정점 셰이더와 픽셀 셰이더 인터페이스로 사용


	// 버퍼로 부터 정점 셰이더를 생성
	result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(),
		vertexShaderBuffer->GetBufferSize(), NULL, &m_vertexShader);
	if (FAILED(result))
	{
		return false;
	}

	// 버퍼로 부터 픽셀 셰이더를 생성
	result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(),
		pixelShaderBuffer->GetBufferSize(), NULL, &m_pixelShader);
	if (FAILED(result))
	{
		return false;
	}


	// 셰이더에서 사용할 정점 데이터의 레이아웃 생성
	// 위치 벡터와 색상 벡터를 사용하므로 레이아웃에 각 벡터의 크기를 포함하는 두 레이아웃을 만듬
	// 사용하는 요소 빼곤 전부 기본값으로


	// 정점 입력 레이아웃 description을 작성
	// 이 설정은 ModelClass와 셰이더에 있는 VertexType과 일치해야함
	// SemanticName : 요소가 레이아웃에서 어떻게 사용되는지 알려줌, 가장 먼저 해야함
	polygonLayout[0].SemanticName = "POSITION";		// 위치 벡터
	polygonLayout[0].SemanticIndex = 0;
	// 위치 벡터와 색상 벡터는 다른 값을 사용함
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	// 버퍼에 데이터가 어떻게 배열되는지, 위치 벡터가 처음 12 byte 사용
	polygonLayout[0].AlignedByteOffset = 0;		// 요소가 어디서 시작하는지 보여줌
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "COLOR";		// 색상 벡터
	polygonLayout[1].SemanticIndex = 0;
	// 위치 벡터와 색상 벡터는 다른 값을 사용함
	polygonLayout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	// 색상 벡터가 나머지 16 byte 사용
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;	// 값을 자동으로 알아내게
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	
	// 레이아웃 description의 크기를 가지고 d3d 디바이스를 사용해 입력 레이아웃을 생성


	// 레이아웃의 요소 개수를 가져옴
	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	// 정점 입력 레이아웃 생성
	result = device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(),
		vertexShaderBuffer->GetBufferSize(), &m_layout);
	if (FAILED(result))
	{
		return false;
	}

	// 더 이상 사용되지 않는 정점 셰이더 버퍼와 픽셀 셰이더 버퍼를 해제
	vertexShaderBuffer->Release();
	vertexShaderBuffer = 0;

	pixelShaderBuffer->Release();
	pixelShaderBuffer = 0;

	
	// 셰이더를 사용하기 위한 마지막 단계 "상수 버퍼(constant buffer)"
	// 지금은 하나의 상수 버퍼를 가지고 있고 그것을 세팅하여 셰이더 인터페이스를 사용가능
	// description이 채워지면 상수 버퍼의 인터페이스를 만들고
	// 이와 SetShaderParameters 홤수를 이용해 셰이더의 내부 변수에 접근할 수 있도록함


	// 정점 셰이더에 있는 행렬 상수 버퍼의 description을 작성
	// 매 프레임마다 상수버퍼를 업데이트 하기 때문에 버퍼의 사용은 동적이여야함
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	// BindFlags로 상수 버퍼를 이 버퍼로 한다고 설정
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	// CPUAccessFlags도 용도에 맞춰야 하기 때문에 설정
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	// 상수 버퍼 포인터를 만들어 이 클래스에서 정점 셰이더 버퍼에 접근할 수 있게함
	result = device->CreateBuffer(&matrixBufferDesc, NULL, &m_matrixBuffer);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}


// 4개의 인터페이스들을 정리
void ColorShaderClass::ShutdownShader()
{
	// 상수 버퍼를 해제
	if (m_matrixBuffer)
	{
		m_matrixBuffer->Release();
		m_matrixBuffer = 0;
	}

	// 레이아웃을 해제
	if (m_layout)
	{
		m_layout->Release();
		m_layout = 0;
	}

	// 픽셀 셰이더를 해제
	if (m_pixelShader)
	{
		m_pixelShader->Release();
		m_pixelShader = 0;
	}

	// 정점 셰이더를 해제
	if (m_vertexShader)
	{
		m_vertexShader->Release();
		m_vertexShader = 0;
	}

	return;
}


// 정점/픽셀 셰이더 컴파일 도중 오류로 인해 생긴 에러 메시지를 출력
void ColorShaderClass::OutputShaderErrorMessage(ID3D10Blob* errorMessage,
	HWND hwnd, const WCHAR* shaderFilename)
{
	char* compileErorrs;
	unsigned long bufferSize, i;
	ofstream fout;

	// 에러 메시지를 담고 있는 문자열 버퍼의 포인터를 가져옴
	compileErorrs = (char*)(errorMessage->GetBufferPointer());

	// 메시지의 길이를 가져옴
	bufferSize = errorMessage->GetBufferSize();

	// 파일을 열고 안에 메시지를 기록함
	fout.open("shader-error.txt");

	// 에러 미시지 작성
	for (i = 0; i < bufferSize; i++)
	{
		fout << compileErorrs[i];
	}

	fout.close();

	// 에러 미시지 반환
	errorMessage->Release();
	errorMessage = 0;

	// 컴파일 에러가 있음을 팝업으로 알려줌
	MessageBox(hwnd, L"Error compiling shader. Check shader-error.txt for message",
		shaderFilename, MB_OK);

	return;
}


// 셰이더의 전역 변수를 쉽게 다룰 수 있도록 하기위함
// 행렬들은 GraphicsClass에서 만들어짐
bool ColorShaderClass::SetShaderParameters(ID3D11DeviceContext* deviceContext,
	XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	unsigned int bufferNumber;

	// 행렬을 transpose해 셰이더에서 사용할 수 있게 함
	// D3DXMatrixTranspose -> XMMatrixTranspose
	worldMatrix = XMMatrixTranspose(worldMatrix);
	viewMatrix = XMMatrixTranspose(viewMatrix);
	projectionMatrix = XMMatrixTranspose(projectionMatrix);

	// 상수 버퍼의 내용을 쓸 수 있도록 잠금
	result = deviceContext->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	// 상수 버퍼의 데이터에 대한 포인터를 가져옴
	dataPtr = (MatrixBufferType*)mappedResource.pData;

	// 상수 버퍼에 행렬을 복사함
	dataPtr->world = worldMatrix;
	dataPtr->view = viewMatrix;
	dataPtr->projection = projectionMatrix;

	// 상수 버퍼의 잠금을 품
	deviceContext->Unmap(m_matrixBuffer, 0);

	// 정점 셰이더에서의 상수 버퍼의 위치를 설정
	bufferNumber = 0;

	// 정점 셰이더의 상수 버퍼를 바뀐 값으로 바꿈
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);

	return true;
}


// Render 함수에서 불리는 두번째 함수
// SetShaderParameters 보다 먼저 호출되어 셰이더 인자들을 올바르게 세팅
void ColorShaderClass::RenderShader(ID3D11DeviceContext* deviceContext, int indexCount)
{
	// 정점 입력 레이아웃을 설정. 입력 레이아웃을 입력 어셈블러에 연결
	// 이것으로 GPU 정점 버퍼의 자료구조를 알게 됨
	deviceContext->IASetInputLayout(m_layout);

	// 삼각형을 그릴 정점 셰이더와 픽셀 셰이더를 설정
	deviceContext->VSSetShader(m_vertexShader, NULL, 0);
	deviceContext->PSSetShader(m_pixelShader, NULL, 0);

	// 삼각형을 그림
	deviceContext->DrawIndexed(indexCount, 0, 0);

	return;
}