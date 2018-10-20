#include "sys.h"

void TerrainShader::init()
{
	HRESULT r;
	ID3D10Blob* error = nullptr;
	ID3D10Blob* vertexBlob = nullptr;
	ID3D10Blob* pixelBlob = nullptr;


	r = D3DX11CompileFromFile(
		this->vsPath.c_str(),
		nullptr,
		nullptr,
		"main",
		"vs_5_0",
		D3D10_SHADER_ENABLE_STRICTNESS,
		0,
		nullptr,
		&vertexBlob,
		&error,
		nullptr
	);

	if (FAILED(r))
	{
		if (error)
		{
			std::cout << (char*)error->GetBufferPointer() << std::endl;
		}
		else
		{
			std::cout << "Fail doesn't exist" << std::endl;
		}

		throw std::runtime_error("");
	}

	r = rend_getDevice()->CreateVertexShader(
		vertexBlob->GetBufferPointer(),
		vertexBlob->GetBufferSize(),
		nullptr,
		&this->vertexShader
	);

	if (FAILED(r))
	{
		std::cout << "Failed to create Vertex Shader" << std::endl;
		throw std::runtime_error("");
	}

	r = D3DX11CompileFromFile(
		this->psPath.c_str(),
		nullptr,
		nullptr,
		"main",
		"ps_5_0",
		D3D10_SHADER_ENABLE_STRICTNESS,
		0,
		nullptr,
		&pixelBlob,
		&error,
		nullptr
	);

	if (FAILED(r))
	{
		if (error)
		{
			std::cout << (char*)error->GetBufferPointer() << std::endl;
		}
		else
		{
			std::cout << "Fail doesn't exist" << std::endl;
		}

		throw std::runtime_error("");
	}

	r = rend_getDevice()->CreatePixelShader(
		pixelBlob->GetBufferPointer(),
		pixelBlob->GetBufferSize(),
		nullptr,
		&this->pixelShader
	);

	if (FAILED(r))
	{
		std::cout << "Failed to create Pixel Shader" << std::endl;
		throw std::runtime_error("");
	}

	std::vector<D3D11_INPUT_ELEMENT_DESC> elements = 
	{
		{
			"POSITION",
			0,
			DXGI_FORMAT_R32G32B32_FLOAT,
			0,
			0,
			D3D11_INPUT_PER_VERTEX_DATA,
			0
		},
		{
			"TEXCOORD",
			0,
			DXGI_FORMAT_R32G32_FLOAT,
			1,
			0,
			D3D11_INPUT_PER_VERTEX_DATA,
			0
		}
	};

	r = rend_getDevice()->CreateInputLayout(
		elements.data(),
		elements.size(),
		vertexBlob->GetBufferPointer(),
		vertexBlob->GetBufferSize(),
		&this->inputLayout
	);

	if (FAILED(r))
	{
		std::cout << "Failed to create input layout..." << std::endl;
		throw std::runtime_error("");
	}

	if (error)
	{
		error->Release();
	}

	if (vertexBlob)
	{
		vertexBlob->Release();
	}

	if (pixelBlob)
	{
		pixelBlob->Release();
	}
}

void TerrainShader::release()
{
	if (inputLayout)
	{
		inputLayout->Release();
	}
	if (pixelShader)
	{
		pixelShader->Release();
	}
	if (vertexShader)
	{
		vertexShader->Release();
	}
}

void TerrainShader::setVSConstBuffer(ID3D11Buffer* constBuffer, int inputSlot)
{
	rend_getContext()->VSSetConstantBuffers(inputSlot, 1, &constBuffer);
}

void TerrainShader::setPSConstBuffer(ID3D11Buffer* constBuffer, int inputSlot)
{
	rend_getContext()->PSSetConstantBuffers(inputSlot, 1, &constBuffer);
}

void TerrainShader::setPSShaderResources(ID3D11ShaderResourceView* resource, int inputSlot)
{
	rend_getContext()->PSSetShaderResources(inputSlot, 1, &resource);
}

void TerrainShader::setPSSamplers(ID3D11SamplerState* samplers, int inputSlot)
{
	rend_getContext()->PSSetSamplers(inputSlot, 1, &samplers);
}

void TerrainShader::bind()
{
	rend_getContext()->VSSetShader(this->vertexShader, nullptr, 0);
	rend_getContext()->PSSetShader(this->pixelShader, nullptr, 0);
	rend_getContext()->IASetInputLayout(this->inputLayout);
}

void TerrainShader::render(Terrain& terrain)
{
	terrain.render();
}