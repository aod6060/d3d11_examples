#pragma once

struct Mesh
{
	// Vertices
	ID3D11Buffer* verticesBuffer = nullptr;
	uint32_t verticeCount = 0;
	// TexCoords
	ID3D11Buffer* texCoordBuffer = nullptr;
	// Normals
	ID3D11Buffer* normalsBuffer = nullptr;
	// Index
	ID3D11Buffer* indicesBuffer = nullptr;
	uint32_t indexCount = 0;

	void init(std::string path);
	void release();

	void createBuffer(std::vector<glm::vec2>& vs, ID3D11Buffer** buf);
	void createBuffer(std::vector<glm::vec3>& vs, ID3D11Buffer** buf);
	void createBuffer(std::vector<uint32_t>& is, ID3D11Buffer** buf);

	void render();

};

struct MeshShader
{
	std::string vsPath = "data/terrain/mesh_vs.hlsl";
	std::string psPath = "data/terrain/mesh_ps.hlsl";

	ID3D11VertexShader* vertexShader = nullptr;
	ID3D11PixelShader* pixelShader = nullptr;
	ID3D11InputLayout* inputLayout = nullptr;

	void init();
	void release();

	void setVSConstBuffer(ID3D11Buffer* constBuffer, int inputSlot);
	void setPSConstBuffer(ID3D11Buffer* constBuffer, int inputSlot);
	void setPSShaderResources(ID3D11ShaderResourceView* resource, int inputSlot);
	void setPSSamplers(ID3D11SamplerState* samplers, int inputSlot);

	void bind();

	void render(Mesh& mesh);

};