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
	// Buffers
	// Vertices
	std::vector<glm::vec3> vertices = {
		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(-1.0f, -1.0f, 0.0f),
		glm::vec3(1.0f, -1.0f, 0.0f)
	};
	ID3D11Buffer* vertexBuffer = nullptr;
	// Colors
	std::vector<glm::vec4> colors = {
		glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),
		glm::vec4(0.0f, 1.0f, 0.0f, 1.0f),
		glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)
	};
	ID3D11Buffer* colorBuffer = nullptr;
	// Textures
	std::vector<glm::vec2> texcoords = {
		glm::vec2(0.5f, 0.0f),
		glm::vec2(0.0f, 1.0f),
		glm::vec2(1.0f, 1.0f)
	};
	ID3D11Buffer* texcoordBuffer = nullptr;
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

	void initBuffer();
	void releaseBuffer();

	void initTextures();
	void releaseTextures();

public:
	virtual void init();
	virtual void update(float delta);
	virtual void render();
	virtual void release();
};