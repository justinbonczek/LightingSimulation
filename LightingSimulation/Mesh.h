#ifndef MESH_H
#define MESH_H

#include <d3d11.h>
#include <vector>
#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

#include "Vertex.h"

struct MeshData
{
	std::vector<Vertex> vertices;
	std::vector<UINT> indices;
};

class Mesh
{
public:
	Mesh(const char* filepath, ID3D11Device* dev);
	Mesh(Vertex* vertices, UINT _numVertices, UINT* indices, UINT _numIndices, ID3D11Device* dev);
	Mesh(MeshData& mesh, ID3D11Device* dev);
	~Mesh();

	UINT GetNumVertices();
	UINT GetNumIndices();
	ID3D11Buffer* GetVertexBuffer();
	ID3D11Buffer* GetIndexBuffer();

	UINT numVertices;
	UINT numIndices;

	std::vector<Vertex> _vertices;
	std::vector<UINT>  _indices;
private:
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
	
	void ProcessScene(aiNode* node, const aiScene* scene);
	void ProcessMesh(aiMesh* mesh, const aiScene* scene);
};

#endif