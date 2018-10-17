#include "sys.h"


// Swap Chain
static IDXGISwapChain* swapChain = nullptr;
// Device
static ID3D11Device* device = nullptr;
// Context
static ID3D11DeviceContext* context = nullptr;
// Render Target View
static ID3D11RenderTargetView* renderTargetView = nullptr;
// Depth Buffer
static ID3D11Texture2D* depthBuffer = nullptr;
// Buffer
static ID3D11DepthStencilState* depthState = nullptr;
// View
static ID3D11DepthStencilView* depthView = nullptr;

void rend_init()
{
	HRESULT r;

	SDL_SysWMinfo info;
	SDL_VERSION(&info.version);
	SDL_GetWindowWMInfo(app_getWindow(), &info);
	// Swap Chain, Device, Context
	DXGI_SWAP_CHAIN_DESC desc = {};
	desc.BufferCount = 1;
	desc.BufferDesc.Width = app_getWidth();
	desc.BufferDesc.Height = app_getHeight();
	desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.BufferDesc.RefreshRate.Numerator = 0;
	desc.BufferDesc.RefreshRate.Denominator = 1;
	desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.OutputWindow = info.info.win.window;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Windowed = TRUE;
	desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	desc.Flags = 0;
	// Create SwapChain, Device, and Context
	r = D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		0,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&desc,
		&swapChain,
		&device,
		nullptr,
		&context
	);
	if (FAILED(r))
	{
		throw std::runtime_error("");
	}
	// Render Target View
	ID3D11Texture2D* backBuffer = nullptr;
	r = swapChain->GetBuffer(
		0,
		__uuidof(ID3D11Texture2D),
		(void**)&backBuffer
	);
	if (FAILED(r))
	{
		throw std::runtime_error("");
	}
	r = device->CreateRenderTargetView(
		backBuffer,
		nullptr,
		&renderTargetView
	);
	if (FAILED(r))
	{
		throw std::runtime_error("");
	}
	if (backBuffer)
	{
		backBuffer->Release();
	}
	// Depth Buffer Desc
	D3D11_TEXTURE2D_DESC depthBufferDesc = {};
	depthBufferDesc.Width = app_getWidth();
	depthBufferDesc.Height = app_getHeight();
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;
	r = device->CreateTexture2D(
		&depthBufferDesc,
		nullptr,
		&depthBuffer
	);
	if (FAILED(r))
	{
		throw std::runtime_error("");
	}
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {};
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilDesc.StencilEnable = false;
	r = device->CreateDepthStencilState(&depthStencilDesc, &depthState);
	if (FAILED(r))
	{
		throw std::runtime_error("");
	}
	D3D11_DEPTH_STENCIL_VIEW_DESC depthViewDesc = {};
	depthViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthViewDesc.Texture2D.MipSlice = 0;
	r = device->CreateDepthStencilView(
		depthBuffer,
		&depthViewDesc,
		&depthView
	);
	if (FAILED(r))
	{
		std::runtime_error("");
	}
	context->OMSetDepthStencilState(depthState, 1);
	context->OMSetRenderTargets(1, &renderTargetView, depthView);
}

void rend_release()
{
	if (depthView)
	{
		depthView->Release();
	}
	if (depthState)
	{
		depthState->Release();
	}
	if (depthBuffer)
	{
		depthBuffer->Release();
	}
	if (renderTargetView)
	{
		renderTargetView->Release();
	}
	if (context)
	{
		context->Release();
	}
	if (device)
	{
		device->Release();
	}
	if (swapChain)
	{
		swapChain->Release();
	}
}

void rend_clear(const glm::vec4& color, float depth)
{
	context->ClearRenderTargetView(renderTargetView, (float*)&color);
	context->ClearDepthStencilView(depthView, D3D11_CLEAR_DEPTH, 1.0F, 0);
}

void rend_present()
{
	swapChain->Present(0, 0);
}

ID3D11Device* rend_getDevice()
{
	return device;
}

ID3D11DeviceContext* rend_getContext()
{
	return context;
}
