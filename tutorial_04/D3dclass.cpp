/*
 * D3dclass.cpp : ��ü ����� ����ϱ� ����
 * ��ī�ϴ� ���̺귯������ ���
 * ���̺귯������ DirectX �ʱ�ȭ, 3D �׷����� ������
 * FPS�� ��ų� GPU���� ���� �ϵ������� ���
 * �� ���� �ʿ��� ��� Direc3D ��ɵ��� ��� ����
 * �ۼ��� : 09.27.21
 * ������ ������Ʈ : 10.02.21
 */
#include "d3dclass.h"
#include <fstream>

// ��� ��� �����͵��� null�� �ʱ�ȭ
D3DClass::D3DClass()
{
	m_swapChain = 0;
	m_device = 0;
	m_deviceContext = 0;
	m_renderTargetView = 0;
	m_depthStencilBuffer = 0;
	m_depthStencilState = 0;
	m_depthStencilView = 0;
	m_rasterState = 0;
}

D3DClass::D3DClass(const D3DClass& other)
{

}

D3DClass::~D3DClass()
{

}

// DirectX 11�� ��ü Direct3D ������ �Ͼ
// �߿��� �ڵ� �Ӹ� �ƴ϶� ���� Ʃ�丮���� ���� �߰��� �ڵ�
// screenWidth, screenHeight : SystemClass���� ���� ������ �ʺ�� ����
// hwnd : �����쿡 ���� �ڵ�, �̰� �̿��� �����쿡 ����
// fullscreen : ��üȭ�� ���� ����, �����츦 �ùٷ� ������ ������ �ʿ�
// screenDepth, screenNear : �����쿡 �׷��� 3Dȯ�濡���� ���̰�
// vsync : Direct3D �������� ��������ȭ �� ������
bool D3DClass::Initialize(int screenWidth,
	int screenHeight, bool vsync, HWND hwnd,
	bool fullscreen, float screenDepth, float screenNear)
{
	HRESULT result;
	IDXGIFactory* factory;
	IDXGIAdapter* adapter;
	IDXGIOutput* adapterOutput;
	unsigned int numModes, i, numerator, denominator, stringLength;
	DXGI_MODE_DESC* displayModeList;
	DXGI_ADAPTER_DESC adapterDesc;
	int error;
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	D3D_FEATURE_LEVEL featureLevel;
	ID3D11Texture2D* backBufferPtr;
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	D3D11_RASTERIZER_DESC rasterDesc;
	D3D11_VIEWPORT viewport;
	float fieldOfView, screenAspect;

	// vsync ������ ������
	m_vsync_enabled = vsync;


	// Direct3D�� �ʱ�ȭ�ϱ� ���� GPU/����� �ֽ����� �˾ƾ���


	// DirectX �׷��� �������̽� ���丮�� ����
	result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
	if (FAILED(result))
	{
		return false;
	}

	// ���丮 ��ü�� ����Ͽ� ù��° gpu �������̽��� ���� ����͸� ����
	result = factory->EnumAdapters(0, &adapter);
	if (FAILED(result))
	{
		return false;
	}

	// ��¿� ���� ù��° ����͸� ����
	result = adapter->EnumOutputs(0, &adapterOutput);
	if (FAILED(result))
	{
		return false;
	}

	// DXGI_FORMAT_R8G8B8A8_UNORM ����� ��� ���÷��� ���信 �´�
	// ����� ������ ����
	numModes = 0;
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_B8G8R8A8_UNORM,
		DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
	if (FAILED(result))
	{
		return false;
	}

	// ������ ��� ����Ϳ� �׷���ī�� ������ ������ ����Ʈ ����
	displayModeList = new DXGI_MODE_DESC[numModes];
	if (!displayModeList)
	{
		return false;
	}

	// ���÷��� ��忡 ���� ����Ʈ ������ ä������
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM,
		DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
	if (FAILED(result))
	{
		return false;
	}

	// ��� ���÷��� ��忡 ���� ȭ�� �ʺ�/���̿� �´� ���÷��� ��带 ã��
	// ������ ���� ã���� ������� ���ΰ�ħ ������ �и�� ���ڰ��� ����
	numerator = 0;
	denominator = 0;
	
	for (i = 0; i < numModes; i++)
	{ 
		if (displayModeList[i].Width == (unsigned int)screenWidth) 
		{ 
			if (displayModeList[i].Height == (unsigned int)screenHeight) 
			{ 
				numerator = displayModeList[i].RefreshRate.Numerator; 
				denominator = displayModeList[i].RefreshRate.Denominator; 
			} 
		} 
	}



	// ���������� �׷��� ī���� �̸��� 
	// ��� ������ �׷��� ī�� �޸��� ũ�⸦ ����͸� �̿��� �޾ƿ�


	// �����(�׷���ī��)�� description�� ������
	result = adapter->GetDesc(&adapterDesc);
	if (FAILED(result))
	{
		return false;
	}

	// ���� �׷��� ī���� �޸� �뷮�� MB ������ ����
	m_videoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

	// �׷��� ī���� �̸��� char�� ���ڿ� �迭�� �ٲ� �� ����
	error = wcstombs_s(&stringLength, m_videoCardDescription,
		128, adapterDesc.Description, 128);
	if (error != 0)
	{
		return false;
	}


	// ����� ���ΰ�ħ ������ ����,�и𰪰� GPU ������ ������Ƿ�
	// ����� ����ü��� �������̽����� ����
	 

	// ���÷��� ��� ����Ʈ �Ҵ��� ����
	delete[] displayModeList;
	displayModeList = 0;

	// ��� ����� �Ҵ� ��ü
	adapterOutput->Release();
	adapterOutput = 0;

	// ����� �Ҵ� ����
	adapter->Release();
	adapter = 0;

	// ���丮 ��ü �Ҵ� ����
	factory->Release();
	factory = 0;


	// �ý��ۿ��� ���� �ֻ����� DirectX�� �ʱ�ȭ
	// ���� ü���� description ����ü�� ä������
	// ���� ü���� ���� �������� �� ���� ��ϵǴ� ����Ʈ���ۿ� �����
	// ���� �������� �� �� �ϳ��� ����۸� ����Ͽ�
	// �� ���� ����� �׸� ��, ����Ʈ ���ۿ� swap�Ͽ� ȭ�鿡 ���̰Ե�


	// ���� ü�� description�� �ʱ�ȭ
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	// �ϳ��� ����۸��� ����ϵ��� �մϴ�.
	swapChainDesc.BufferCount = 1;

	// ������� �ʺ�� ���̸� ����
	swapChainDesc.BufferDesc.Width = screenWidth;
	swapChainDesc.BufferDesc.Height = screenHeight;

	// ����۷� �Ϲ����� 32bit�� �����̽��� ����
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;


	// �ֻ��� ����. 1�ʴ� �� ���� ����� ����Ʈ���ۿ� �ٲ�ġ�� �ϴ���
	// vsync�� true��� ��������ȭ on
	// vsync�� false��� ���α׷��� 1�ʿ� �ִ��� ������ ȭ���� �׸�����
	// �̰��� ȭ�� �Ϻο� ������ ����� ��


	// ������� ���ΰ�ħ ������ ����
	if (m_vsync_enabled)
	{
		// ��������ȭ on
		swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
	}
	else
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}

	// ������� �뵵�� ����
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	// �������� �̷���� �������� �ڵ� ����
	swapChainDesc.OutputWindow = hwnd;

	// ��Ƽ ���ø��� ��
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	// ������ ��� �Ǵ� Ǯ��ũ�� ��带 ����
	if (fullscreen)
	{
		swapChainDesc.Windowed = false;
	}
	else
	{
		swapChainDesc.Windowed = true;
	}

	// ��ĵ������ ���İ� ��ĵ���̴��� �������� ����(unspecified)���� ����
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// ��µ� ������ ������� ������ ����������
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	// �߰� �ɼ� �÷��׸� ������� ����
	swapChainDesc.Flags = 0;


	// ���� �����̶�� ������ ����
	// �츮�� ��� ������ DirectX�� ����� ������ �˷���


	// ���� ������ DirectX 11�� ����
	featureLevel = D3D_FEATURE_LEVEL_11_0;


	// ����ü��, Direct3D ��ġ, Direct3D ��ġ ���ؽ�Ʈ�� ���� �� ����
	// ��ġ, ��ġ ���ؽ�Ʈ�� ��� Direct3D���� �������̽��� ��
	// �������� ��ġ�� ��ġ ���ؽ�Ʈ�� ����� ��κ��� �۾��� ����
	// DirectX 11�� �������� �ʴ� �׷���ī���� ���� �Լ��� ����
	// ���� ���ٸ� D3D_DRIVER_TYPE_HARDWARE�� D3D_DRIVER_TYPE_REFERENCE�� ����
	// �̸� ���� CPU �������� �ϰ� ������ ����


	// ����ü��, Direct3D ����̽�, Direct3D ����̽� ���ý�Ʈ ����
	result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL,
		0, &featureLevel, 1, D3D11_SDK_VERSION, &swapChainDesc, &m_swapChain,
		&m_device, NULL, &m_deviceContext);
	if (FAILED(result))
	{
		return false;
	}


	// ������� �����͸� �޾ƿ� ����ü�ο� �����Ŵ

	
	// ������� �����͸� ������
	result = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);
	if (FAILED(result))
	{
		return false;
	}

	// ������� �����ͷ� ����Ÿ�� �並 ����
	result = m_device->CreateRenderTargetView(backBufferPtr, NULL, &m_renderTargetView);
	if (FAILED(result))
	{
		return false;
	}

	// ����� �����͸� ���̻� ������� �����Ƿ� �Ҵ� ����
	backBufferPtr->Release();
	backBufferPtr = 0;


	// ���� ������ description ����ü �ۼ�
	// �̸� �̿��� ���� ���۸� ������ 3D �������� �������� �ùٸ��� �׷���
	// ���ٽ� ���۵� �� ���� ���ۿ� ������ ��Ǻ�, ������Ʈ�� �׸����� ȿ���� ��


	// ���� ������ description�� �ʱ�ȭ
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	// ���� ������ description�� �ۼ�
	depthBufferDesc.Width = screenWidth;
	depthBufferDesc.Height = screenHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;


	// ������� ����/���Ľ� ���۸� ����
	// ���� ���� 2D �ؽ�����
	// ��������� ���ĵǰ� ������ȭ �� ���Ŀ� 2D ��ǥ�� �ȼ����� �Ǳ� ����


	// desription�� ����Ͽ� ���� ������ �ؽ�ó�� ����
	result = m_device->CreateTexture2D(&depthBufferDesc, NULL, &m_depthStencilBuffer);
	if (FAILED(result))
	{
		return false;
	}


	// ����-���ٽ� description�� �ۼ�
	// �츮�� direct3D���� �� �ȼ��� � ���� �ؽ�Ʈ�� �� ������ ���� �� �ְԵ�


	// ���ٽ� ������ description�� �ʱ�ȭ
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	// ���ٽ� ������ description�� �ۼ�
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// �ȼ��� ����Ʈ�н��� ����� ���ٽ� �Լ�
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// �ȼ��� ���н��� ����� ���ٽ� �Լ�
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// ����-���ٽ� ���� ������ ����
	result = m_device->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilState);
	if (FAILED(result))
	{
		return false;
	}

	// ����-���ٽ� ���¸� ����
	m_deviceContext->OMSetDepthStencilState(m_depthStencilState, 1);

	
	// ����-���ٽ� ������ �信 ���� description
	// �� �۾��� �ؾ� direct3D�� ���� ���۸� ����-���ٽ� �ؽ��ķ� �ν���
	// �� ����ü�� ä��� �� �Ŀ� CreateDepthStencilVIew �Լ��� ȣ���� �並 ����


	// ����-���ٽ� ���� description�� �ʱ�ȭ
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	// ����-���ٽ� ���� description�� �ۼ�
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// ����-���ٽ� �並 ����
	result = m_device->CreateDepthStencilView(m_depthStencilBuffer,
		&depthStencilViewDesc, &m_depthStencilView);
	if (FAILED(result))
	{
		return false;
	}


	// OMSetRenderTarget �Լ��� ����Ÿ�� ��� ����-���ٽ� �並
	// ��� ������ ���������ο� ���ε���Ŵ
	// ������������ �̿��� �������� ����� �� ������� ����ۿ� ����� �׷���
	

	// ����Ÿ�� ��� ����-���ٽ� ���۸� ���� ��� ���������ο� ���ε�
	m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);

	
	// ����Ÿ���� ������ �� �������� ������ ���� �߰����� ��ɵ� ���� ����
	// ������ȭ�� ����(rasterizer state) : ������ ��� �ȼ��� �׷����� ���� ����
	// ��� �� �� �ְ� ����. ȭ���� ���̾������� ���� �׸��ų�
	// ������ �յ޸��� ��� �׸����� �� �� ����
	// DirectX �⺻ ������ȭ�� ��������, ���� ������ �ʴ� �̻� ������� ����

	
	// � ������ ��� �׸� ������ �����ϴ� ������ȭ�� description
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;


	// �ۼ��� description���� ���� ������ȭ�� ���¸� ����
	result = m_device->CreateRasterizerState(&rasterDesc, &m_rasterState);
	if (FAILED(result))
	{
		return false;
	}

	// ������ȭ�� ���¸� ����
	m_deviceContext->RSSetState(m_rasterState);


	// ����Ʈ�� �־�� ����Ÿ�� �������� Ŭ���� ���� ����
	// ������ ��ü ũ��� �����ϰ� ����


	// �������� ���� ����Ʈ����
	viewport.Width = (float)screenWidth;
	viewport.Height = (float)screenHeight;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

	// ����Ʈ ����
	m_deviceContext->RSSetViewports(1, &viewport);


	// ���� ���(projection matrix)����
	// ���� ����� 3D ȭ���� �ռ� ���� 2D ����Ʈ �������� ��ȯ�ϵ��� ����
	// ����� ���纻�� ����� ���̴����� ����� �׸� �� ����� �� �ֵ��� �ؾ���
	

	// ���� ����� ����
	fieldOfView = (float)XM_PI / 4.0f;
	screenAspect = (float)screenWidth / (float)screenHeight;

	// 3D �������� ���� ���� ����� ����
	//D3DXMatrixPerspectiveFovLH(&m_projectionMatrix, fieldOfView,
	//	screenAspect, screenNear, screenDepth);
	m_projectionMatrix = XMMatrixPerspectiveFovLH(fieldOfView,
		screenAspect, screenNear, screenDepth);


	// ���� ����� ����. ������Ʈ���� ��ǥ�� 3D ���� ��ǥ�� ��ȯ�ϴµ� ���
	// 3���� ���������� ȸ��/�̵�/ũ�� ��ȯ���� ���
	// �� ����� ���� ��ķ� ����� ���纻�� ����
	// ���纻�� ���̴��� ���޵Ǿ� ����� �� �ְ� ��


	// ���� ����� ���� ��ķ� �ʱ�ȭ
	//D3DXMatrixIdentity(&m_worldMatrix);
	m_worldMatrix = XMMatrixIdentity();


	// ���� �並 ����. �� ����� ���� ��鿡�� 
	// ��� ��ġ���� ��� ������ ���� �ִ°��� ����ϴ� �� ����
	// 3D ������ ī�޶� ���� ���, ���߿� ����
	
	// ������ �۾��� ���� ���� ����� ����� ��
	// �� ����� 3D��ü�� �ƴ� UI�� ���� 2D ��ҵ��� �׸��� ���� ���
	// 2D �׷��Ȱ� ��Ʈ�� �ٷ�� ������ ���


	// 2D �������� ���� ���� ���� ����� ����
	//D3DXMatrixOrthoLH(&m_orthoMatrix, (float)screenWidth, (float)screenHeight,
	//	screenNear, screenDepth);
	m_orthoMatrix = XMMatrixOrthographicLH((float)screenWidth, (float)screenHeight,
		screenNear, screenDepth);

	return true;
}


// Init���� ����� �����͵��� �����ϰ� ����
// ���� ���� ü���� ������ ���� �ٲٴ� �Լ��� ȣ��
// �� �۾��� ���� �ʰ� Ǯ��ũ�� ���¿��� ����ü���� �����ϸ�
// ��� ���ܰ� �߻��ϱ� ������ Direct3D�� �����ϱ� ����
// ������ ���� ü���� ������ ���� �ٲ����
void D3DClass::Shutdown()
{
	// �����ϱ� ���� �̷��� ������� �ٲ��� ������
	// ����ü���� �Ҵ� ��ü�� �� ���ܰ� �߻�
	if (m_swapChain)
	{
		m_swapChain->SetFullscreenState(false, NULL);
	}

	if (m_rasterState)
	{
		m_rasterState->Release();
		m_rasterState = 0;
	}

	if (m_depthStencilView)
	{
		m_depthStencilView->Release();
		m_depthStencilView = 0;
	}

	if (m_depthStencilState)
	{
		m_depthStencilState->Release();
		m_depthStencilState = 0;
	}

	if (m_depthStencilBuffer)
	{
		m_depthStencilBuffer->Release();
		m_depthStencilBuffer = 0;
	}

	if (m_renderTargetView)
	{
		m_renderTargetView->Release();
		m_renderTargetView = 0;
	}

	if (m_deviceContext)
	{
		m_deviceContext->Release();
		m_deviceContext = 0;
	}

	if (m_device)
	{
		m_device->Release();
		m_device = 0;
	}

	if (m_swapChain)
	{
		m_swapChain->Release();
		m_swapChain = 0;
	}

	return;
}


// �� �������� ���۸��� 3D ȭ���� �׸��� ������ �� ȣ��
// ���۸� �� ������ �ʱ�ȭ�ϰ� �������� �̷�������� �غ�
void D3DClass::BeginScene(float red, float green, float blue, float alpha)
{
	float color[4];

	// ���۸� � �������� ���� ������ ����
	color[0] = red;
	color[1] = green;
	color[2] = blue;
	color[3] = alpha;

	// ������� ������ ����
	m_deviceContext->ClearRenderTargetView(m_renderTargetView, color);

	// ���� ������ ����
	m_deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH,
		1.0f, 0);

	return;
}

// �� �������� �������� ���� ü�ο��� ����ۿ� �׸� 3Dȭ���� �����ϵ��� �ϰ� ��
void D3DClass::EndScene()
{
	// �������� �Ϸ�Ǿ����Ƿ� ������� ������ ȭ�鿡 ǥ��
	if (m_vsync_enabled)
	{
		// ���ΰ�ħ ������ ����
		m_swapChain->Present(1, 0);
	}
	else
	{
		// ������ ������ ǥ��
		m_swapChain->Present(0, 0);
	}

	return;
}

// ����� �Լ���

// �ܼ��� Direct3D ����̽� �����͸� �������� �Լ�
// �����ӿ�ũ���� ���� ȣ���
ID3D11Device* D3DClass::GetDevice()
{
	return m_device;
}


// �ܼ��� Direct3D ����̽� ���ؽ�Ʈ �����͸� �������� �Լ�
// �����ӿ�ũ���� ���� ȣ���
ID3D11DeviceContext* D3DClass::GetDeviceContext()
{
	return m_deviceContext;
}

// ���� ����� ��ȯ
// ��κ��� ���̴������� ������� �ʿ����
void D3DClass::GetProjectionMatrix(XMMATRIX& projectionMatrix)
{
	projectionMatrix = m_projectionMatrix;
	return;
}

// ���� ����� ��ȯ
// ��κ��� ���̴������� ������� �ʿ����
void D3DClass::GetWorldMatrix(XMMATRIX& worldMatrix)
{
	worldMatrix = m_worldMatrix;
	return;
}

// ���� ���� ����� ��ȯ
// ��κ��� ���̴������� ������� �ʿ����
void D3DClass::GetOrthoMatrix(XMMATRIX& orthoMatrix)
{
	orthoMatrix = m_orthoMatrix;
	return;
}

// �׷��� ī�� �̸��� ��밡���� �޸� ���� ��ȯ
// ���� �ٸ� �������� ������ϴ� �� �����
void D3DClass::GetVideoCardInfo(char* cardName, int& memory)
{
	strcpy_s(cardName, 128, m_videoCardDescription);
	memory = m_videoCardMemory;

	return;
}