#pragma once

#include "sys.h"

struct ConstVS
{
	glm::mat4 proj;
	glm::mat4 view;
	glm::mat4 model;
};

struct ConstPS
{
	float tcScale;
	glm::vec3 unused1;
};

class AppTest : public IApp
{
private:
	// Camera
	Camera camera;
	// Shaders
	ID3D11VertexShader* vertexShader = nullptr;
	ID3D11PixelShader* pixelShader = nullptr;
	// Input Layout
	ID3D11InputLayout* inputLayout = nullptr;

	// Terrain Shader
	TerrainShader terrainShader;

	// Terrain
	Terrain terrain;

	// Mesh Shader
	MeshShader meshShader;

	// Mesh
	Mesh cube;
	Mesh sphere;
	Mesh cylender;
	Mesh torus;
	Mesh monkey;

	std::vector<Mesh*> meshes;
	uint32_t amount = 0;

	// Const Buffer
	// ConstVS
	ConstVS constVS = {};
	ID3D11Buffer* constVSBuffer = nullptr;

	// ConstPS
	ConstPS constPS = {};
	ID3D11Buffer* constPSBuffer = nullptr;

	// Rasterizer
	ID3D11RasterizerState* rastState = nullptr;
	// View Port
	D3D11_VIEWPORT viewport = {};
	// Rotation Values
	float yrot = 0.0f;
	// Texture
	ID3D11Texture2D* exampleTex = nullptr;
	ID3D11ShaderResourceView* exampleTex0 = nullptr;
	ID3D11SamplerState* exampleSampState = nullptr;

	ID3D11Texture2D* grassTex = nullptr;
	ID3D11ShaderResourceView* grassTex0 = nullptr;
	ID3D11SamplerState* grassSampState = nullptr;

	void initMesh();
	void releaseMesh();

	void initTextures();
	void releaseTextures();

	void updateConstBuffer(ID3D11Buffer* cBuf, void* data, size_t size);

public:
	virtual void init();
	virtual void update(float delta);
	virtual void render();
	virtual void release();
};