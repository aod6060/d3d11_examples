#include "test.h"

void AppTest::init()
{
	rend_init();
	this->initShader();
	this->initMesh();
	this->initTextures();

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

	if (input_isKeyDown(Keyboard::KEY_TAB))
	{
		this->amount += 1;
	}

	if (this->meshes.size() <= this->amount)
	{
		this->amount = 0;
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
		glm::rotate(glm::mat4(1.0f), glm::radians(yrot), glm::vec3(1.0f, 1.0f, 0.0f));

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
	rend_getContext()->PSSetShaderResources(0, 1, &this->exampleTex0);
	rend_getContext()->PSSetSamplers(0, 1, &this->exampleSampState);
	rend_getContext()->IASetInputLayout(this->inputLayout);

	meshes[this->amount]->render();

	rend_present();
}

void AppTest::release()
{
	if (rastState)
	{
		rastState->Release();
	}
	this->releaseTextures();
	this->releaseMesh();
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
		"data/simple_camera/shader_vs.hlsl",
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
		"data/simple_camera/shader_ps.hlsl",
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
		}, 
		{
			"TEXCOORD",
			0,
			DXGI_FORMAT_R32G32_FLOAT,
			1,
			0,
			D3D11_INPUT_PER_VERTEX_DATA,
			0
		}, 
		{
			"NORMAL",
			0,
			DXGI_FORMAT_R32G32B32_FLOAT,
			2,
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

void AppTest::initMesh()
{
	cube.init("data/simple_camera/cube.blend");
	sphere.init("data/simple_camera/sphere.blend");
	cylender.init("data/simple_camera/cylender.blend");
	torus.init("data/simple_camera/torus.blend");
	monkey.init("data/simple_camera/monkey.blend");

	meshes.push_back(&cube);
	meshes.push_back(&sphere);
	meshes.push_back(&cylender);
	meshes.push_back(&torus);
	meshes.push_back(&monkey);
}

void AppTest::releaseMesh()
{
	meshes.clear();
	monkey.release();
	torus.release();
	cylender.release();
	sphere.release();
	cube.release();
}

void AppTest::initTextures()
{
	HRESULT r;

	r = D3DX11CreateShaderResourceViewFromFile(
		rend_getDevice(),
		"data/simple_camera/example.png",
		nullptr,
		nullptr,
		&this->exampleTex0,
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

	r = rend_getDevice()->CreateSamplerState(&sampDesc, &this->exampleSampState);

	if (FAILED(r))
	{
		std::cout << "Falied to create sampler state..." << std::endl;
		throw std::runtime_error("");
	}
}

void AppTest::releaseTextures()
{
	if (this->exampleSampState)
	{
		this->exampleSampState->Release();
	}
	if (this->exampleTex0)
	{
		this->exampleTex0->Release();
	}
}