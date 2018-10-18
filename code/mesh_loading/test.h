#pragma once

#include "sys.h"

struct ConstVS
{
	glm::mat4 proj;
	glm::mat4 view;
	glm::mat4 model;
};

class AppTest : public IApp
{
private:
	// Shaders
	ID3D11VertexShader* vertexShader = nullptr;
	ID3D11PixelShader* pixelShader = nullptr;
	// Input Layout
	ID3D11InputLayout* inputLayout = nullptr;
	// Mesh
	Mesh cube;
	Mesh sphere;
	Mesh cylender;
	Mesh torus;
	Mesh monkey;

	std::vector<Mesh*> meshes;
	uint32_t amount = 0;

	// Const Buffer
	ID3D11Buffer* constVSBuffer = nullptr;
	// Rasterizer
	ID3D11RasterizerState* rastState = nullptr;
	// View Port
	D3D11_VIEWPORT viewport = {};
	// Rotation Values
	float yrot = 0.0f;
	// Texture
	ID3D11ShaderResourceView* exampleTex0 = nullptr;
	ID3D11SamplerState* exampleSampState = nullptr;

	void initShader();
	void releaseShader();

	void initMesh();
	void releaseMesh();

	void initTextures();
	void releaseTextures();

public:
	virtual void init();
	virtual void update(float delta);
	virtual void render();
	virtual void release();
};