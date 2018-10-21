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

	example.init("data/skybox/textures/example.png");
	//sand.init("data/terrain_mt/textures/sand.png");

	// Terrain Textures
	blendMap.init("data/skybox/terrain/blend_map.png");

	channelBlack.init("data/skybox/textures/rock.png");
	channelRed.init("data/skybox/textures/sand.png");
	channelGreen.init("data/skybox/textures/thick_grass.png");
	channelBlue.init("data/skybox/textures/grass.png");

	terrainShader.init();
	terrain.init("data/skybox/terrain/height_map_island.png");

	// Create Const VS
	constVSBuf.init();
	// Create Const PS
	constPSBuf.init();

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

	constPSBuf.set(constPS);
	constPSBuf.update();

	terrainShader.bind();
	terrainShader.setVSConstBuffer(this->constVSBuf.buf, 0);

	//sand.bind(0);

	terrainShader.setBlendMap(this->blendMap);
	terrainShader.setChannelBlack(this->channelBlack);
	terrainShader.setChannelRed(this->channelRed);
	terrainShader.setChannelGreen(this->channelGreen);
	terrainShader.setChannelBlue(this->channelBlue);

	terrainShader.setPSConstBuffer(this->constPSBuf.buf, 0);

	constVS.model =
		glm::translate(glm::mat4(1.0f), glm::vec3(0.0f));

	constVSBuf.set(constVS);
	constVSBuf.update();
	terrainShader.render(terrain);

	meshShader.bind();
	meshShader.setVSConstBuffer(this->constVSBuf.buf, 0);
	example.bind(0);

	// Back
	constVS.model =
		glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.0f)) *
		glm::rotate(glm::mat4(1.0f), glm::radians(yrot), glm::vec3(1.0f, 1.0f, 0.0f));
	constVSBuf.set(constVS);
	constVSBuf.update();
	meshShader.render(*meshes[this->amount]);
	// Front
	constVS.model =
		glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 5.0f)) *
		glm::rotate(glm::mat4(1.0f), glm::radians(yrot), glm::vec3(1.0f, 1.0f, 0.0f));
	constVSBuf.set(constVS);
	constVSBuf.update();
	meshShader.render(*meshes[this->amount]);
	// Left
	constVS.model =
		glm::translate(glm::mat4(1.0f), glm::vec3(-5.0f, 0.0f, 0.0f)) *
		glm::rotate(glm::mat4(1.0f), glm::radians(yrot), glm::vec3(1.0f, 1.0f, 0.0f));
	constVSBuf.set(constVS);
	constVSBuf.update();
	meshShader.render(*meshes[this->amount]);
	// Right
	constVS.model =
		glm::translate(glm::mat4(1.0f), glm::vec3(5.0f, 0.0f, 0.0f)) *
		glm::rotate(glm::mat4(1.0f), glm::radians(yrot), glm::vec3(1.0f, 1.0f, 0.0f));
	constVSBuf.set(constVS);
	constVSBuf.update();
	meshShader.render(*meshes[this->amount]);
	// Bottom
	constVS.model =
		glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -5.0f, 0.0f)) *
		glm::rotate(glm::mat4(1.0f), glm::radians(yrot), glm::vec3(1.0f, 1.0f, 0.0f));
	constVSBuf.set(constVS);
	constVSBuf.update();
	meshShader.render(*meshes[this->amount]);
	// Up
	constVS.model =
		glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 5.0f, 0.0f)) *
		glm::rotate(glm::mat4(1.0f), glm::radians(yrot), glm::vec3(1.0f, 1.0f, 0.0f));
	constVSBuf.set(constVS);
	constVSBuf.update();
	meshShader.render(*meshes[this->amount]);

	rend_present();
}

void AppTest::release()
{
	if (rastState)
	{
		rastState->Release();
	}
	constPSBuf.release();
	constVSBuf.release();

	channelBlue.release();
	channelGreen.release();
	channelRed.release();
	channelBlack.release();
	blendMap.release();

	example.release();
	terrain.release();
	terrainShader.release();
	this->releaseMesh();
	meshShader.release();
	rend_release();
}

void AppTest::initMesh()
{
	cube.init("data/skybox/mesh/cube.blend");
	sphere.init("data/skybox/mesh/sphere.blend");
	cylender.init("data/skybox/mesh/cylender.blend");
	torus.init("data/skybox/mesh//torus.blend");
	monkey.init("data/skybox/mesh//monkey.blend");

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
