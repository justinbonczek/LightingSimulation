#include "MeshGenerator.h"
#include <vector>

void MeshGenerator::CreateSphere(float radius, UINT numSubdivisions, MeshData& data)
{
	numSubdivisions = min(numSubdivisions, 5u);

	const float X = 0.525731f;
	const float Z = 0.850651f;

	XMFLOAT3 pos[12] =
	{
		XMFLOAT3(-X, 0.0f, Z), XMFLOAT3(X, 0.0f, Z),
		XMFLOAT3(-X, 0.0f, -Z), XMFLOAT3(X, 0.0f, -Z),
		XMFLOAT3(0.0f, Z, X), XMFLOAT3(0.0f, Z, -X),
		XMFLOAT3(0.0f, -Z, X), XMFLOAT3(0.0f, -Z, -X),
		XMFLOAT3(Z, X, 0.0f), XMFLOAT3(-Z, X, 0.0f),
		XMFLOAT3(Z, -X, 0.0f), XMFLOAT3(-Z, -X, 0.0f)
	};

	UINT ind[60] =
	{
		1, 4, 0, 4, 9, 0, 4, 5, 9, 8, 5, 4, 1, 8, 4,
		1, 10, 8, 10, 3, 8, 8, 3, 5, 3, 2, 5, 3, 7, 2,
		3, 10, 7, 10, 6, 7, 6, 11, 7, 6, 0, 11, 6, 1, 0,
		10, 1, 6, 11, 0, 9, 2, 11, 9, 5, 2, 9, 11, 2, 7
	};

	data.vertices.resize(12);
	data.indices.resize(60);

	for (byte i = 0; i < 12; i++)
	{
		data.vertices[i].Position = pos[i];
	}

	for (byte i = 0; i < 60; i++)
	{
		data.indices[i] = ind[i];
	}

	for (byte i = 0; i < numSubdivisions; i++)
	{
		Subdivide(data);
	}

	for (int i = 0; i < data.vertices.size(); ++i)
	{
		XMVECTOR n = XMVector3Normalize(XMLoadFloat3(&data.vertices[i].Position));

		XMVECTOR p = radius * n;

		XMStoreFloat3(&data.vertices[i].Position, p);
		XMStoreFloat3(&data.vertices[i].Normal, n);

		float theta = atan2(data.vertices[i].Position.x, data.vertices[i].Position.z);

		float phi = acosf(data.vertices[i].Position.y / radius);

		data.vertices[i].UV.x = theta / XM_2PI;
		data.vertices[i].UV.y = phi / XM_PI;

		data.vertices[i].Tangent.x = -radius*sinf(phi)*sinf(theta);
		data.vertices[i].Tangent.y = 0.0f;
		data.vertices[i].Tangent.z = +radius*sinf(phi)*cosf(theta);

		XMVECTOR T = XMLoadFloat3(&data.vertices[i].Tangent);
		XMStoreFloat3(&data.vertices[i].Tangent, XMVector3Normalize(T));
	}
}

void MeshGenerator::Subdivide(MeshData& mesh)
{
	MeshData copy = mesh;

	mesh.vertices.resize(0);
	mesh.indices.resize(0);

	UINT numTris = copy.indices.size() / 3;
	for (UINT i = 0; i < numTris; ++i)
	{
		Vertex v0 = copy.vertices[copy.indices[i * 3 + 0]];
		Vertex v1 = copy.vertices[copy.indices[i * 3 + 1]];
		Vertex v2 = copy.vertices[copy.indices[i * 3 + 2]];

		Vertex m0, m1, m2;

		m0.Position = XMFLOAT3(
			0.5f*(v0.Position.x + v1.Position.x),
			0.5f*(v0.Position.y + v1.Position.y),
			0.5f*(v0.Position.z + v1.Position.z));

		m1.Position = XMFLOAT3(
			0.5f*(v1.Position.x + v2.Position.x),
			0.5f*(v1.Position.y + v2.Position.y),
			0.5f*(v1.Position.z + v2.Position.z));

		m2.Position = XMFLOAT3(
			0.5f*(v0.Position.x + v2.Position.x),
			0.5f*(v0.Position.y + v2.Position.y),
			0.5f*(v0.Position.z + v2.Position.z));

		mesh.vertices.push_back(v0); // 0
		mesh.vertices.push_back(v1); // 1
		mesh.vertices.push_back(v2); // 2
		mesh.vertices.push_back(m0); // 3
		mesh.vertices.push_back(m1); // 4
		mesh.vertices.push_back(m2); // 5

		mesh.indices.push_back(i * 6 + 0);
		mesh.indices.push_back(i * 6 + 3);
		mesh.indices.push_back(i * 6 + 5);

		mesh.indices.push_back(i * 6 + 3);
		mesh.indices.push_back(i * 6 + 4);
		mesh.indices.push_back(i * 6 + 5);

		mesh.indices.push_back(i * 6 + 5);
		mesh.indices.push_back(i * 6 + 4);
		mesh.indices.push_back(i * 6 + 2);

		mesh.indices.push_back(i * 6 + 3);
		mesh.indices.push_back(i * 6 + 1);
		mesh.indices.push_back(i * 6 + 4);
	}
}


void MeshGenerator::CreatePlane(float width, float depth, UINT n, UINT m, MeshData& data)
{
	UINT vertexCount = n * m;
	UINT faceCount = (n - 1)*(m - 1) * 2;

	float halfWidth = width * 0.5f;
	float halfDepth = depth * 0.5f;

	float dx = width / (n - 1);
	float dz = depth / (m - 1);

	float du = 1.0f / (n - 1);
	float dv = 1.0f / (m - 1);

	data.indices.resize(0);
	data.vertices.resize(0);

	for (int i = 0; i < m; ++i)
	{
		float z = halfDepth - i * dz;
		for (int j = 0; j < n; ++j)
		{
			float x = halfWidth - j * dx;
			Vertex cVert;
			cVert.Position = XMFLOAT3(x, 1.0, z);
			cVert.Normal = XMFLOAT3(0.0, 1.0, 0.0);
			cVert.Tangent = XMFLOAT3(1.0f, 0.0f, 0.0f);
			cVert.UV.x = j * du;
			cVert.UV.y = i * dv;
			cVert.Color = XMFLOAT4(0.7, 0.7, 0.7, 1.0);
			data.vertices.push_back(cVert);
		}
	}

	UINT k = 0;
	for (int i = 0; i < m - 1; ++i)
	{
		for (int j = 0; j < n - 1; ++j)
		{
			data.indices.push_back((i + 1) * n + j + 1);
			data.indices.push_back(i * n + j + 1);
			data.indices.push_back((i + 1) * n + j);
			data.indices.push_back((i + 1) * n + j);
			data.indices.push_back(i * n + j + 1);
			data.indices.push_back(i*n + j);
			k += 6;
		}
	}
}