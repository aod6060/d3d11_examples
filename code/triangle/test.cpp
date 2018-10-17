#include "test.h"

void AppTest::init()
{
	rend_init();
	this->initShader();
	this->initBuffer();

	D3D11_RASTERIZER_DESC rastDesc = {};
	rastDesc.AntialiasedLineEnable = false;
	rastDesc.CullMode = D3D11_CULL_NONE;
	rastDesc.DepthBias = 0;
	rastDesc.DepthBiasClamp = 0.0f;
	rastDesc.DepthClipEnable = true;
	rastDesc.FillMode = D3D11_FILL_SOLID;
	rastDesc.FrontCounterClockwise = false;
	rastDesc.MultisampleEnable = false;
	rastDesc.ScissorEnable = false;
	rastDesc.SlopeScaledDepthBias = 0.0f;

	HRESULT r = rend_getDevice()->CreateRasterizerState(
		&rastDesc,
		&this->rastState
	);

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = app_getWidth();
	viewport.Height = app_getHeight();
	viewport.MaxDepth = 1.0f;
	viewport.MinDepth = 0.0f;
}

void AppTest::update(float delta)
{
	yrot += 64.0f * delta;
	if (yrot > 360.0f)
	{
		yrot -= 360.0f;
	}
}

void AppTest::render()
{
	ConstVS constVS = {};
	constVS.proj =
		glm::perspective(glm::radians(45.0f), (float)app_getWidth() / (float)app_getHeight(), 1.0f, 1024.0f);
	constVS.view =
		glm::mat4(1.0f);
	constVS.model =
		glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.0f)) *
		glm::rotate(glm::mat4(1.0f), glm::radians(yrot), glm::vec3(0.0f, 1.0f, 0.0f));

	D3D11_MAPPED_SUBRESOURCE mapped = {};
	rend_getContext()->Map(constVSBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
	memcpy(mapped.pData, &constVS, sizeof(ConstVS));
	rend_getContext()->Unmap(constVSBuffer, 0);

	rend_clear(glm::vec4(glm::vec3(135.0f, 206.0f, 235.0f) / 255.0f, 1.0f), 1.0f);

	rend_getContext()->RSSetState(this->rastState);
	rend_getContext()->RSSetViewports(1, &viewport);

	rend_getContext()->VSSetShader(this->vertexShader, nullptr, 0);
	rend_getContext()->VSSetConstantBuffers(0, 1, &this->constVSBuffer);
	rend_getContext()->PSSetShader(this->pixelShader, nullptr, 0);

	uint32_t stride = sizeof(glm::vec3);
	uint32_t offset = 0;

	rend_getContext()->IASetInputLayout(this->inputLayout);
	rend_getContext()->IASetVertexBuffers(0, 1, &this->vertexBuffer, &stride, &offset);
	rend_getContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	rend_getContext()->Draw(vertices.size(), 0);

	rend_present();
}

void AppTest::release()
{
	if (rastState)
	{
		rastState->Release();
	}
	this->releaseBuffer();
	this->releaseShader();
	rend_release();
}

void AppTest::initShader()
{
	HRESULT r;
	ID3D10Blob* error = nullptr;
	ID3D10Blob* vertexBlob = nullptr;
	ID3D10Blob* pixelBlob = nullptr;

	r = D3DX11CompileFromFile(
		"data/triangle/shader_vs.hlsl",
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
		&this->vertexShader);

	if (FAILED(r))
	{
		std::cout << "Failed to create Vertex Shader" << std::endl;
		throw std::runtime_error("");
	}

	r = D3DX11CompileFromFile(
		"data/triangle/shader_ps.hlsl",
		nullptr,
		nullptr,
		"main",
		"ps_5_0",
		D3D10_SHADER_ENABLE_STRICTNESS,
		0,
		nullptr,
		&pixelBlob,
		&error,
		nullptr);

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
		std::cout << "Failed to create Vertex Shader" << std::endl;
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
		}
	};

	r = rend_getDevice()->CreateInputLayout(
		elements.data(),
		elements.size(),
		vertexBlob->GetBufferPointer(),
		vertexBlob->GetBufferSize(),
		&this->inputLayout);

	if (FAILED(r))
	{
		std::cout << "Failed to create input layout..." << std::endl;
		throw std::runtime_error("");
	}

	// VS Buffer
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth = sizeof(ConstVS);
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	r = rend_getDevice()->CreateBuffer(
		&bufferDesc,
		nullptr,
		&this->constVSBuffer
	);

	if (FAILED(r))
	{
		std::cout << "Falied to create constVSBuffer..." << std::endl;
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

void AppTest::releaseShader()
{
	if (this->constVSBuffer)
	{
		this->constVSBuffer->Release();
	}
	if (this->inputLayout)
	{
		this->inputLayout->Release();
	}
	if (this->vertexShader)
	{
		this->vertexShader->Release();
	}
	if (this->pixelShader)
	{
		this->pixelShader->Release();
	}
}

void AppTest::initBuffer() 
{
	HRESULT r;

	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(glm::vec3) * vertices.size();
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA subData = {};
	subData.pSysMem = vertices.data();
	subData.SysMemPitch = 0;
	subData.SysMemSlicePitch = 0;

	r = rend_getDevice()->CreateBuffer(
		&bufferDesc,
		&subData,
		&this->vertexBuffer
	);

	if (FAILED(r))
	{
		std::cout << "Failed to Vertex Buffer!" << std::endl;
		throw std::runtime_error("");
	}
}

void AppTest::releaseBuffer()
{
	if (vertexBuffer)
	{
		vertexBuffer->Release();
	}
}

