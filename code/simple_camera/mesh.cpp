#include "sys.h"

void Mesh::init(std::string path)
{
	Assimp::Importer imp;

	const aiScene* scene = imp.ReadFile(
		path.c_str(),
		aiProcess_Triangulate);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << imp.GetErrorString() << std::endl;
		throw std::runtime_error("");
	}

	const aiNode* root = scene->mRootNode;

	uint32_t numMesh = root->mNumMeshes;
	uint32_t numChild = root->mNumChildren;

	const aiNode* child = root->mChildren[root->mNumChildren - 1];

	aiMesh* mesh = scene->mMeshes[child->mMeshes[child->mNumMeshes - 1]];

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> texCoords;
	std::vector<glm::vec3> normals;

	for (uint32_t i = 0; i < mesh->mNumVertices; i++)
	{
		// Vertices
		glm::vec3 v;
		v.x = mesh->mVertices[i].x;
		v.y = mesh->mVertices[i].z;
		v.z = mesh->mVertices[i].y;
		vertices.push_back(v);
		// Normals
		glm::vec3 vn;
		vn.x = mesh->mNormals[i].x;
		vn.y = mesh->mNormals[i].z;
		vn.z = mesh->mNormals[i].y;
		normals.push_back(vn);
		// TexCoords
		glm::vec2 vt;
		vt.x = mesh->mTextureCoords[0][i].x;
		vt.y = mesh->mTextureCoords[0][i].y;
		texCoords.push_back(vt);
	}

	std::vector<uint32_t> indices;

	for (uint32_t i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (uint32_t j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	this->indexCount = indices.size();
	this->verticeCount = vertices.size();

	// Vertices
	this->createBuffer(vertices, &verticesBuffer);
	// Normals
	this->createBuffer(normals, &normalsBuffer);
	// TexCoords
	this->createBuffer(texCoords, &texCoordBuffer);
	// Indices
	this->createBuffer(indices, &indicesBuffer);

}

void Mesh::release()
{
	if (indicesBuffer)
	{
		indicesBuffer->Release();
	}
	if (texCoordBuffer)
	{
		texCoordBuffer->Release();
	}
	if (normalsBuffer)
	{
		normalsBuffer->Release();
	}
	if (verticesBuffer)
	{
		verticesBuffer->Release();
	}
}

void Mesh::createBuffer(std::vector<glm::vec2>& vs, ID3D11Buffer** buf)
{
	D3D11_BUFFER_DESC desc = {};
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = sizeof(glm::vec2) * vs.size();
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA data = {};
	data.pSysMem = vs.data();
	data.SysMemPitch = 0;
	data.SysMemSlicePitch = 0;

	HRESULT r = rend_getDevice()->CreateBuffer(
		&desc,
		&data,
		buf
	);

	if (FAILED(r))
	{
		std::cout << "Something wrong" << std::endl;
		throw std::runtime_error("");
	}
}

void Mesh::createBuffer(std::vector<glm::vec3>& vs, ID3D11Buffer** buf)
{
	D3D11_BUFFER_DESC desc = {};
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = sizeof(glm::vec3) * vs.size();
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA data = {};
	data.pSysMem = vs.data();
	data.SysMemPitch = 0;
	data.SysMemSlicePitch = 0;

	HRESULT r = rend_getDevice()->CreateBuffer(
		&desc,
		&data,
		buf
	);

	if (FAILED(r))
	{
		std::cout << "Something wrong" << std::endl;
		throw std::runtime_error("");
	}
}

void Mesh::createBuffer(std::vector<uint32_t>& is, ID3D11Buffer** buf)
{
	D3D11_BUFFER_DESC desc = {};
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = sizeof(uint32_t) * is.size();
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA data = {};
	data.pSysMem = is.data();
	data.SysMemPitch = 0;
	data.SysMemSlicePitch = 0;

	HRESULT r = rend_getDevice()->CreateBuffer(
		&desc,
		&data,
		buf
	);
}

void Mesh::render()
{
	uint32_t stride = sizeof(glm::vec3);
	uint32_t offset = 0;
	rend_getContext()->IASetVertexBuffers(0, 1, &verticesBuffer, &stride, &offset);
	stride = sizeof(glm::vec2);
	rend_getContext()->IASetVertexBuffers(1, 1, &texCoordBuffer, &stride, &offset);
	stride = sizeof(glm::vec3);
	rend_getContext()->IASetVertexBuffers(2, 1, &normalsBuffer, &stride, &offset);
	rend_getContext()->IASetIndexBuffer(indicesBuffer, DXGI_FORMAT_R32_UINT, 0);
	rend_getContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	rend_getContext()->DrawIndexed(indexCount, 0, 0);
}
