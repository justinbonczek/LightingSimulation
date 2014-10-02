#include "Mesh.h"
#include <vector>
#include "Material.h"
#include "Game.h"

Mesh::Mesh(const char* filepath, ID3D11Device* dev)
{
	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(filepath, aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType);

	ProcessScene(scene->mRootNode, scene);

	numVertices = _vertices.size();
	numIndices = _indices.size();

	Vertex * vertices = &_vertices[0];
	UINT * indices = &_indices[0];

	D3D11_BUFFER_DESC vb;
	ZeroMemory(&vb, sizeof(D3D11_BUFFER_DESC));
	vb.Usage = D3D11_USAGE_IMMUTABLE;
	vb.ByteWidth = sizeof(Vertex) * numVertices;
	vb.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vb.CPUAccessFlags = 0;
	vb.MiscFlags = 0;
	vb.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA initVertData;
	initVertData.pSysMem = vertices;
	dev->CreateBuffer(
		&vb,
		&initVertData,
		&vertexBuffer);

	D3D11_BUFFER_DESC ib;
	ZeroMemory(&ib, sizeof(D3D11_BUFFER_DESC));
	ib.Usage = D3D11_USAGE_IMMUTABLE;
	ib.ByteWidth = sizeof(UINT) * numIndices;
	ib.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ib.CPUAccessFlags = 0;
	ib.MiscFlags = 0;
	ib.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA initIndexData;
	initIndexData.pSysMem = indices;
	dev->CreateBuffer(
		&ib,
		&initIndexData,
		&indexBuffer);
}

Mesh::Mesh(Vertex* vertices, UINT numVertices, UINT* indices, UINT numIndices, ID3D11Device* dev):
numVertices(numVertices),
numIndices(numIndices)
{
	D3D11_BUFFER_DESC vb;
	ZeroMemory(&vb, sizeof(D3D11_BUFFER_DESC));
	vb.Usage = D3D11_USAGE_IMMUTABLE;
	vb.ByteWidth = sizeof(Vertex) * numVertices;
	vb.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vb.CPUAccessFlags = 0;
	vb.MiscFlags = 0;
	vb.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA initVertData;
	initVertData.pSysMem = vertices;
	dev->CreateBuffer(
		&vb,
		&initVertData,
		&vertexBuffer);

	D3D11_BUFFER_DESC ib;
	ZeroMemory(&ib, sizeof(D3D11_BUFFER_DESC));
	ib.Usage = D3D11_USAGE_IMMUTABLE;
	ib.ByteWidth = sizeof(UINT) * numIndices;
	ib.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ib.CPUAccessFlags = 0;
	ib.MiscFlags = 0;
	ib.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA initIndexData;
	initIndexData.pSysMem = indices;
	dev->CreateBuffer(
		&ib,
		&initIndexData,
		&indexBuffer);
}

Mesh::Mesh(MeshData& mesh, ID3D11Device* dev)
{
	_vertices = mesh.vertices;
	_indices = mesh.indices;

	Vertex* vertices = &mesh.vertices[0];
	UINT*   indices  = &mesh.indices[0];

	numVertices = mesh.vertices.size();
	numIndices = mesh.indices.size();

	D3D11_BUFFER_DESC vb;
	ZeroMemory(&vb, sizeof(D3D11_BUFFER_DESC));
	vb.Usage = D3D11_USAGE_IMMUTABLE;
	vb.ByteWidth = sizeof(Vertex) * mesh.vertices.size();
	vb.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vb.CPUAccessFlags = 0;
	vb.MiscFlags = 0;
	vb.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA initVertData;
	initVertData.pSysMem = vertices;
	dev->CreateBuffer(
		&vb,
		&initVertData,
		&vertexBuffer);

	D3D11_BUFFER_DESC ib;
	ZeroMemory(&ib, sizeof(D3D11_BUFFER_DESC));
	ib.Usage = D3D11_USAGE_IMMUTABLE;
	ib.ByteWidth = sizeof(UINT) * mesh.indices.size();
	ib.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ib.CPUAccessFlags = 0;
	ib.MiscFlags = 0;
	ib.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA initIndexData;
	initIndexData.pSysMem = indices;
	dev->CreateBuffer(
		&ib,
		&initIndexData,
		&indexBuffer);
}

Mesh::~Mesh()
{
	ReleaseMacro(vertexBuffer);
	ReleaseMacro(indexBuffer);
}


void Mesh::ProcessScene(aiNode* node, const aiScene* scene)
{
	for (int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[i];
		ProcessMesh(mesh, scene);
	}

	for (int i = 0; i < node->mNumChildren; i++)
	{
		ProcessScene(node->mChildren[i], scene);
	}
}

void Mesh::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	for (int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex temp;
		XMFLOAT3 tempvec;

		// Vertices
		tempvec.x = mesh->mVertices[i].x;
		tempvec.y = mesh->mVertices[i].y;
		tempvec.z = mesh->mVertices[i].z;
		temp.Position = tempvec;

		// Normals
		tempvec.x = mesh->mNormals[i].x;
		tempvec.y = mesh->mNormals[i].y;
		tempvec.z = mesh->mNormals[i].z;
		temp.Normal = tempvec;

		// Tangents
		if (mesh->mTangents)
		{
			tempvec.x = mesh->mTangents[i].x;
			tempvec.y = mesh->mTangents[i].y;
			tempvec.z = mesh->mTangents[i].z;
			
		}
		else
		{
			tempvec.x = 1.0;
			tempvec.y = tempvec.z = 0.0;
		}
		temp.Tangent = tempvec;

		// Colors
		if (mesh->mColors[0])
		{
			tempvec.x = mesh->mColors[0][i].r;
			tempvec.y = mesh->mColors[0][i].g;
			tempvec.z = mesh->mColors[0][i].b;
			
		}
		else
		{
			tempvec.x = tempvec.y = tempvec.z = 0.7;
		}
		temp.Color = XMFLOAT4(tempvec.x, tempvec.y, tempvec.z, 1.0);

		// UVs
		if (mesh->mTextureCoords[0])
		{
			tempvec.x = mesh->mTextureCoords[0][i].x;
			tempvec.y = mesh->mTextureCoords[0][i].y;
		}
		else
		{
			tempvec.x = tempvec.y = tempvec.z = 0.0;
		}
		temp.UV.x = tempvec.x;
		temp.UV.y = tempvec.y;
		_vertices.push_back(temp);
	}

	for (int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (int j = 0; j < face.mNumIndices; j++)
		{
			_indices.push_back(face.mIndices[j]);
		}
	}
}

UINT Mesh::GetNumVertices(){ return numVertices; }
UINT Mesh::GetNumIndices(){ return numIndices; }
ID3D11Buffer* Mesh::GetVertexBuffer(){ return vertexBuffer; }
ID3D11Buffer* Mesh::GetIndexBuffer(){ return indexBuffer;}