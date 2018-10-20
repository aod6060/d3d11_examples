#include "sys.h"

void Texture2D::init(std::string path)
{
	HRESULT r;

	r = D3DX11CreateShaderResourceViewFromFile(
		rend_getDevice(),
		path.c_str(),
		nullptr,
		nullptr,
		&this->resView,
		nullptr
	);

	if (FAILED(r))
	{
		std::cout << "Failed to load shader resource view" << std::endl;
		throw std::runtime_error("");
	}

	D3D11_SAMPLER_DESC sampDesc = {};
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	r = rend_getDevice()->CreateSamplerState(&sampDesc, &this->sampState);

	if (FAILED(r))
	{
		std::cout << "Falied to create sampler state..." << std::endl;
		throw std::runtime_error("");
	}
}

void Texture2D::release()
{
	if (sampState)
	{
		sampState->Release();
	}
	if (resView)
	{
		resView->Release();
	}
}

void Texture2D::bind(int inputState)
{
	rend_getContext()->PSSetShaderResources(inputState, 1, &this->resView);
	rend_getContext()->PSSetSamplers(inputState, 1, &this->sampState);
}
