#include "test.h"

void AppTest::init()
{
	this->camera.init(
		glm::vec3(0.0f),
		glm::vec2(0.0f),
		glm::radians(45.0f),
		(float)app_getWidth() / (float)app_getHeight(),
		1.0f,
		1024.0f);

	rend_init();

	meshShader.init();

	this->initMesh();
	this->initTextures();

	terrainShader.init();
	terrain.init("data/terrain_mt/textures/height_map_island.png");

	// Create Const VS
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth = sizeof(ConstVS);
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	HRESULT r = rend_getDevice()->CreateBuffer(
		&bufferDesc,
		nullptr,
		&this->constVSBuffer);

	if (FAILED(r))
	{
		std::cout << "Failed to create Const VS Buffer" << std::endl;
		throw std::runtime_error("");
	}

	// Create Const PS
	D3D11_BUFFER_DESC bufPSDesc = {};
	bufPSDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufPSDesc.ByteWidth = sizeof(ConstPS);
	bufPSDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufPSDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufPSDesc.MiscFlags = 0;
	bufPSDesc.StructureByteStride = 0;

	r = rend_getDevice()->CreateBuffer(
		&bufferDesc,
		nullptr,
		&this->constPSBuffer
	);

	if (FAILED(r))
	{
		std::cout << "Failed to create Const PS Buffer" << std::endl;
		throw std::runtime_error("");
	}

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

	r = rend_getDevice()->CreateRasterizerState(
		&rastDesc,
		&this->rastState
	);

	if (FAILED(r))
	{
		std::cout << "Failed to create rasterizer..." << std::endl;
		throw std::runtime_error("");
	}

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

	if (input_isKeyDown(Keyboard::KEY_1))
	{
		this->amount += 1;
	}

	if (this->meshes.size() <= this->amount)
	{
		this->amount = 0;
	}

	if (input_isKeyDown(Keyboard::KEY_TAB))
	{
		input_toggleGrab();
	}

	if (input_isGrab())
	{
		this->camera.update(delta);
	}
}

void AppTest::render()
{
	rend_clear(glm::vec4(glm::vec3(135.0f, 206.0f, 235.0f) / 255.0f, 1.0f), 1.0f);

	rend_getContext()->RSSetState(this->rastState);
	rend_getContext()->RSSetViewports(1, &viewport);

	constVS.proj = this->camera.toProj();
	constVS.view = this->camera.toView();

	constPS.tcScale = terrain.width / 16.0f;

	this->updateConstBuffer(this->constPSBuffer, &constPS, sizeof(ConstPS));

	terrainShader.bind();
	terrainShader.setVSConstBuffer(this->constVSBuffer, 0);
	terrainShader.setPSShaderResources(this->grassTex0, 0);
	terrainShader.setPSSamplers(this->grassSampState, 0);
	terrainShader.setPSConstBuffer(this->constPSBuffer, 0);

	constVS.model =
		glm::translate(glm::mat4(1.0f), glm::vec3(0.0f));

	this->updateConstBuffer(this->constVSBuffer, &constVS, sizeof(ConstVS));
	terrainShader.render(terrain);

	meshShader.bind();
	meshShader.setVSConstBuffer(this->constVSBuffer, 0);
	meshShader.setPSShaderResources(this->exampleTex0, 0);
	meshShader.setPSSamplers(this->exampleSampState, 0);

	// Back
	constVS.model =
		glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.0f)) *
		glm::rotate(glm::mat4(1.0f), glm::radians(yrot), glm::vec3(1.0f, 1.0f, 0.0f));
	this->updateConstBuffer(this->constVSBuffer, &constVS, sizeof(ConstVS));
	meshShader.render(*meshes[this->amount]);
	// Front
	constVS.model =
		glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 5.0f)) *
		glm::rotate(glm::mat4(1.0f), glm::radians(yrot), glm::vec3(1.0f, 1.0f, 0.0f));
	this->updateConstBuffer(this->constVSBuffer, &constVS, sizeof(ConstVS));
	meshShader.render(*meshes[this->amount]);
	// Left
	constVS.model =
		glm::translate(glm::mat4(1.0f), glm::vec3(-5.0f, 0.0f, 0.0f)) *
		glm::rotate(glm::mat4(1.0f), glm::radians(yrot), glm::vec3(1.0f, 1.0f, 0.0f));
	this->updateConstBuffer(this->constVSBuffer, &constVS, sizeof(ConstVS));
	meshShader.render(*meshes[this->amount]);
	// Right
	constVS.model =
		glm::translate(glm::mat4(1.0f), glm::vec3(5.0f, 0.0f, 0.0f)) *
		glm::rotate(glm::mat4(1.0f), glm::radians(yrot), glm::vec3(1.0f, 1.0f, 0.0f));
	this->updateConstBuffer(this->constVSBuffer, &constVS, sizeof(ConstVS));
	meshShader.render(*meshes[this->amount]);
	// Bottom
	constVS.model =
		glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -5.0f, 0.0f)) *
		glm::rotate(glm::mat4(1.0f), glm::radians(yrot), glm::vec3(1.0f, 1.0f, 0.0f));
	this->updateConstBuffer(this->constVSBuffer, &constVS, sizeof(ConstVS));
	meshShader.render(*meshes[this->amount]);
	// Up
	constVS.model =
		glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 5.0f, 0.0f)) *
		glm::rotate(glm::mat4(1.0f), glm::radians(yrot), glm::vec3(1.0f, 1.0f, 0.0f));
	this->updateConstBuffer(this->constVSBuffer, &constVS, sizeof(ConstVS));
	meshShader.render(*meshes[this->amount]);

	rend_present();
}

void AppTest::release()
{
	if (rastState)
	{
		rastState->Release();
	}
	if (this->constPSBuffer)
	{
		constPSBuffer->Release();
	}
	if (this->constVSBuffer)
	{
		constVSBuffer->Release();
	}
	terrain.release();
	terrainShader.release();
	this->releaseTextures();
	this->releaseMesh();
	meshShader.release();
	rend_release();
}

void AppTest::initMesh()
{
	cube.init("data/terrain_mt/mesh/cube.blend");
	sphere.init("data/terrain_mt/mesh/sphere.blend");
	cylender.init("data/terrain_mt/mesh/cylender.blend");
	torus.init("data/terrain_mt/mesh//torus.blend");
	monkey.init("data/terrain_mt/mesh//monkey.blend");

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
		"data/terrain_mt/textures/example.png",
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

	r = D3DX11CreateShaderResourceViewFromFile(
		rend_getDevice(),
		"data/terrain/sand.png",
		nullptr,
		nullptr,
		&this->grassTex0,
		nullptr);

	if (FAILED(r))
	{
		std::cout << "Failed to load shader resource view" << std::endl;
		throw std::runtime_error("");
	}

	r = rend_getDevice()->CreateSamplerState(
		&sampDesc,
		&this->grassSampState
	);

	if (FAILED(r))
	{
		std::cout << "Falied to create sampler state..." << std::endl;
		throw std::runtime_error("");
	}
}

void AppTest::releaseTextures()
{
	if (grassSampState)
	{
		this->grassSampState->Release();
	}
	if (this->grassTex0)
	{
		this->grassTex0->Release();
	}
	if (this->exampleSampState)
	{
		this->exampleSampState->Release();
	}
	if (this->exampleTex0)
	{
		this->exampleTex0->Release();
	}
}

void AppTest::updateConstBuffer(ID3D11Buffer* cBuf, void* data, size_t size)
{
	D3D11_MAPPED_SUBRESOURCE mapped = {};
	rend_getContext()->Map(cBuf, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
	memcpy(mapped.pData, data, size);
	rend_getContext()->Unmap(cBuf, 0);
}
