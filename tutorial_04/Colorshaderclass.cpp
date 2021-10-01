/*
 * Colorshaderclass.cpp : GPU�� �����ϴ� 3D �𵨵��� �׸��µ� ����ϴ�
 * HLSL ���̴��� ȣ���ϴ� Ŭ����
 * �ۼ��� : 10.01.21
 * ������ ������Ʈ : 10.01.21
 */

//////////////////
// includes
//////////////////
#include "Colorshaderclass.h"

ColorShaderClass::ColorShaderClass()
{
	// �ʱ�ȭ
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

	// ���� ���̴��� �ȼ� ���̴��� �ʱ�ȭ
	// HLSL ���̴� ������ color.vs, color.ps �� �Ѱ���
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
	// ���� ���̴��� �ȼ� ���̴� �� �׿� ���õ� �͵� ����
	ShutdownShader();

	return;
}


bool ColorShaderClass::Render(ID3D11DeviceContext* deviceContext, int indexCount,
	XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix)
{
	bool result;

	// �������� ����� ���̴� ���ڸ� �Է�
	// ���̴����� ���� ���ڵ��� ������
	result = SetShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix);
	if (!result)
	{
		return false;
	}

	// ���̴��� �̿��� �غ�� ���۸� �׸�
	// HLSL ���̴��� �̿��� ��� �ﰢ��
	RenderShader(deviceContext, indexCount);

	return true;
}

// �̹� Ʃ�丮�󿡼� ���� �߿��� �Լ�
// ������ ���̴� ������ �ҷ����� DirectX�� GPU���� ��� �����ϵ��� �ϰ���
// ���̾ƿ��� ����, ��� ���� ������ �����Ͱ� GPU�� ���Ǵ��� �� �� ����
// ���̾ƿ��� modelclass.h �� color.vs �� �����  VertexType�� ��ġ�ؾ���
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

	// �� �Լ����� ���Ǵ� ������ null�� ����
	errorMessage = 0;
	vertexShaderBuffer = 0;
	pixelShaderBuffer = 0;


	// ���̴� ���α׷��� ���۷� ������
	// ���̴� ���� �̸�, ���̴� �̸�, ���̴� ����(�ٷ�11���� 5.0), ������ �� ���۸� ���ڷ�
	// ������ ���н� errorMessage ���ڿ��� ���, �ٸ� �Լ��� �� �� ����
	// errorMessage ���� �����ϸ� ���̴� ���� ��ü�� ���� -> �� �� �ֵ��� ���̾�α� ����


	// ���� ���̴��� ������
	// D3DX11CompileFromFile -> D3DCompileFromFile
	result = D3DCompileFromFile(vsFilename, NULL, NULL, "ColorVertexShader",
		"vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &vertexShaderBuffer, &errorMessage);
	if (FAILED(result))
	{
		// ���̴��� �����Ͽ� �����ϸ� ���� �޽����� ���
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, vsFilename);
		}
		// �����޽����� ���� == ���̴� ������ ã�� ����
		else
		{
			MessageBox(hwnd, vsFilename, L"Missing Shader FIle", MB_OK);
		}

		return false;
	}

	// �ȼ� ���̴��� ������
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

	
	// ������ �� ���̴���� ���̴� ��ü�� ����
	// ���⼭ ���� �����͸� ���� ���̴��� �ȼ� ���̴� �������̽��� ���


	// ���۷� ���� ���� ���̴��� ����
	result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(),
		vertexShaderBuffer->GetBufferSize(), NULL, &m_vertexShader);
	if (FAILED(result))
	{
		return false;
	}

	// ���۷� ���� �ȼ� ���̴��� ����
	result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(),
		pixelShaderBuffer->GetBufferSize(), NULL, &m_pixelShader);
	if (FAILED(result))
	{
		return false;
	}


	// ���̴����� ����� ���� �������� ���̾ƿ� ����
	// ��ġ ���Ϳ� ���� ���͸� ����ϹǷ� ���̾ƿ��� �� ������ ũ�⸦ �����ϴ� �� ���̾ƿ��� ����
	// ����ϴ� ��� ���� ���� �⺻������


	// ���� �Է� ���̾ƿ� description�� �ۼ�
	// �� ������ ModelClass�� ���̴��� �ִ� VertexType�� ��ġ�ؾ���
	// SemanticName : ��Ұ� ���̾ƿ����� ��� ���Ǵ��� �˷���, ���� ���� �ؾ���
	polygonLayout[0].SemanticName = "POSITION";		// ��ġ ����
	polygonLayout[0].SemanticIndex = 0;
	// ��ġ ���Ϳ� ���� ���ʹ� �ٸ� ���� �����
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	// ���ۿ� �����Ͱ� ��� �迭�Ǵ���, ��ġ ���Ͱ� ó�� 12 byte ���
	polygonLayout[0].AlignedByteOffset = 0;		// ��Ұ� ��� �����ϴ��� ������
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "COLOR";		// ���� ����
	polygonLayout[1].SemanticIndex = 0;
	// ��ġ ���Ϳ� ���� ���ʹ� �ٸ� ���� �����
	polygonLayout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	// ���� ���Ͱ� ������ 16 byte ���
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;	// ���� �ڵ����� �˾Ƴ���
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	
	// ���̾ƿ� description�� ũ�⸦ ������ d3d ����̽��� ����� �Է� ���̾ƿ��� ����


	// ���̾ƿ��� ��� ������ ������
	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	// ���� �Է� ���̾ƿ� ����
	result = device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(),
		vertexShaderBuffer->GetBufferSize(), &m_layout);
	if (FAILED(result))
	{
		return false;
	}

	// �� �̻� ������ �ʴ� ���� ���̴� ���ۿ� �ȼ� ���̴� ���۸� ����
	vertexShaderBuffer->Release();
	vertexShaderBuffer = 0;

	pixelShaderBuffer->Release();
	pixelShaderBuffer = 0;

	
	// ���̴��� ����ϱ� ���� ������ �ܰ� "��� ����(constant buffer)"
	// ������ �ϳ��� ��� ���۸� ������ �ְ� �װ��� �����Ͽ� ���̴� �������̽��� ��밡��
	// description�� ä������ ��� ������ �������̽��� �����
	// �̿� SetShaderParameters �c���� �̿��� ���̴��� ���� ������ ������ �� �ֵ�����


	// ���� ���̴��� �ִ� ��� ��� ������ description�� �ۼ�
	// �� �����Ӹ��� ������۸� ������Ʈ �ϱ� ������ ������ ����� �����̿�����
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	// BindFlags�� ��� ���۸� �� ���۷� �Ѵٰ� ����
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	// CPUAccessFlags�� �뵵�� ����� �ϱ� ������ ����
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	// ��� ���� �����͸� ����� �� Ŭ�������� ���� ���̴� ���ۿ� ������ �� �ְ���
	result = device->CreateBuffer(&matrixBufferDesc, NULL, &m_matrixBuffer);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}


// 4���� �������̽����� ����
void ColorShaderClass::ShutdownShader()
{
	// ��� ���۸� ����
	if (m_matrixBuffer)
	{
		m_matrixBuffer->Release();
		m_matrixBuffer = 0;
	}

	// ���̾ƿ��� ����
	if (m_layout)
	{
		m_layout->Release();
		m_layout = 0;
	}

	// �ȼ� ���̴��� ����
	if (m_pixelShader)
	{
		m_pixelShader->Release();
		m_pixelShader = 0;
	}

	// ���� ���̴��� ����
	if (m_vertexShader)
	{
		m_vertexShader->Release();
		m_vertexShader = 0;
	}

	return;
}


// ����/�ȼ� ���̴� ������ ���� ������ ���� ���� ���� �޽����� ���
void ColorShaderClass::OutputShaderErrorMessage(ID3D10Blob* errorMessage,
	HWND hwnd, const WCHAR* shaderFilename)
{
	char* compileErorrs;
	unsigned long bufferSize, i;
	ofstream fout;

	// ���� �޽����� ��� �ִ� ���ڿ� ������ �����͸� ������
	compileErorrs = (char*)(errorMessage->GetBufferPointer());

	// �޽����� ���̸� ������
	bufferSize = errorMessage->GetBufferSize();

	// ������ ���� �ȿ� �޽����� �����
	fout.open("shader-error.txt");

	// ���� �̽��� �ۼ�
	for (i = 0; i < bufferSize; i++)
	{
		fout << compileErorrs[i];
	}

	fout.close();

	// ���� �̽��� ��ȯ
	errorMessage->Release();
	errorMessage = 0;

	// ������ ������ ������ �˾����� �˷���
	MessageBox(hwnd, L"Error compiling shader. Check shader-error.txt for message",
		shaderFilename, MB_OK);

	return;
}


// ���̴��� ���� ������ ���� �ٷ� �� �ֵ��� �ϱ�����
// ��ĵ��� GraphicsClass���� �������
bool ColorShaderClass::SetShaderParameters(ID3D11DeviceContext* deviceContext,
	XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	unsigned int bufferNumber;

	// ����� transpose�� ���̴����� ����� �� �ְ� ��
	// D3DXMatrixTranspose -> XMMatrixTranspose
	worldMatrix = XMMatrixTranspose(worldMatrix);
	viewMatrix = XMMatrixTranspose(viewMatrix);
	projectionMatrix = XMMatrixTranspose(projectionMatrix);

	// ��� ������ ������ �� �� �ֵ��� ���
	result = deviceContext->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	// ��� ������ �����Ϳ� ���� �����͸� ������
	dataPtr = (MatrixBufferType*)mappedResource.pData;

	// ��� ���ۿ� ����� ������
	dataPtr->world = worldMatrix;
	dataPtr->view = viewMatrix;
	dataPtr->projection = projectionMatrix;

	// ��� ������ ����� ǰ
	deviceContext->Unmap(m_matrixBuffer, 0);

	// ���� ���̴������� ��� ������ ��ġ�� ����
	bufferNumber = 0;

	// ���� ���̴��� ��� ���۸� �ٲ� ������ �ٲ�
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);

	return true;
}


// Render �Լ����� �Ҹ��� �ι�° �Լ�
// SetShaderParameters ���� ���� ȣ��Ǿ� ���̴� ���ڵ��� �ùٸ��� ����
void ColorShaderClass::RenderShader(ID3D11DeviceContext* deviceContext, int indexCount)
{
	// ���� �Է� ���̾ƿ��� ����. �Է� ���̾ƿ��� �Է� ������� ����
	// �̰����� GPU ���� ������ �ڷᱸ���� �˰� ��
	deviceContext->IASetInputLayout(m_layout);

	// �ﰢ���� �׸� ���� ���̴��� �ȼ� ���̴��� ����
	deviceContext->VSSetShader(m_vertexShader, NULL, 0);
	deviceContext->PSSetShader(m_pixelShader, NULL, 0);

	// �ﰢ���� �׸�
	deviceContext->DrawIndexed(indexCount, 0, 0);

	return;
}