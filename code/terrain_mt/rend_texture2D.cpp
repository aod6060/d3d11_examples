#include "sys.h"

void Texture2D::init(std::string path)
{
	HRESULT r;

	ImageData imageData;

	img_load(path, &imageData);

	D3D11_TEXTURE2D_DESC texDesc = {};
	texDesc.Width = imageData.width;
	texDesc.Height = imageData.height;
	texDesc.MipLevels = 0;
	texDesc.ArraySize = 1;
	texDesc.CPUAccessFlags = 0;
	texDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	texDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

	r = rend_getDevice()->CreateTexture2D(
		&texDesc,
		nullptr,
		&this->tex
	);

	if (FAILED(r))
	{
		std::cout << "Failed to texture2D" << std::endl;
		throw std::runtime_error("");
	}

	uint32_t rowPitch = imageData.width * 4 * sizeof(uint8_t);

	rend_getContext()->UpdateSubresource(tex, 0, nullptr, imageData.pixels, rowPitch, 0);

	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResView = {};
	shaderResView.Format = texDesc.Format;
	shaderResView.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResView.Texture2D.MipLevels = -1;
	shaderResView.Texture2D.MostDetailedMip = 0;

	r = rend_getDevice()->CreateShaderResourceView(
		tex,
		&shaderResView,
		&this->resView
	);

	if (FAILED(r))
	{
		std::cout << "Failed to load shader resource view" << std::endl;
		throw std::runtime_error("");
	}

	rend_getContext()->GenerateMips(this->resView);

	img_unload(&imageData);

	/*
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
	*/

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
