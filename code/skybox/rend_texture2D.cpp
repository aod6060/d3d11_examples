#include "sys.h"

void Texture2D::init(std::string path)
{
	HRESULT r;

	ImageData imageData;

	img_load(path, &imageData);

	D3D11_TEXTURE2D_DESC tex_desc = {};
	tex_desc.Width = imageData.width;
	tex_desc.Height = imageData.height;
	tex_desc.MipLevels = 0;
	tex_desc.ArraySize = 1;
	tex_desc.CPUAccessFlags = 0;
	tex_desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	tex_desc.SampleDesc.Count = 1;
	tex_desc.SampleDesc.Quality = 0;
	tex_desc.Usage = D3D11_USAGE_DEFAULT;
	tex_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	tex_desc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

	r = rend_getDevice()->CreateTexture2D(
		&tex_desc,
		nullptr,
		&this->tex
	);

	
	if (FAILED(r))
	{
		std::cout << "Failed to texture 2D" << std::endl;
		throw std::runtime_error("");
	}

	uint32_t rowPitch = imageData.width * 4 * sizeof(uint8_t);

	rend_getContext()->UpdateSubresource(tex, 0, nullptr, imageData.pixels, rowPitch, 0);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvd = {};
	srvd.Format = tex_desc.Format;
	srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvd.Texture2D.MipLevels = -1;
	srvd.Texture2D.MostDetailedMip = 0;

	r = rend_getDevice()->CreateShaderResourceView(
		tex,
		&srvd,
		&this->resView
	);

	if (FAILED(r))
	{
		std::cout << "Failed to load shader resource view" << std::endl;
		throw std::runtime_error("");
	}
	
	rend_getContext()->GenerateMips(this->resView);

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

	img_unload(&imageData);
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
	if (tex)
	{
		tex->Release();
	}
}

void Texture2D::bind(int inputState)
{
	rend_getContext()->PSSetShaderResources(inputState, 1, &this->resView);
	rend_getContext()->PSSetSamplers(inputState, 1, &this->sampState);
}
