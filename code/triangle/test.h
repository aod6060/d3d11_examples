#pragma once

#include "sys.h"

class AppTest : public IApp
{
private:
	// Swap Chain
	IDXGISwapChain* swapChain = nullptr;
	// Device
	ID3D11Device* device = nullptr;
	// Context
	ID3D11DeviceContext* context = nullptr;
	// Render Target View
	ID3D11RenderTargetView* renderTargetView = nullptr;
	// Depth Buffer
	ID3D11Texture2D* depthBuffer = nullptr;
	// Buffer
	ID3D11DepthStencilState* depthState = nullptr;
	// View
	ID3D11DepthStencilView* depthView = nullptr;
public:
	virtual void init();
	virtual void update(float delta);
	virtual void render();
	virtual void release();
};