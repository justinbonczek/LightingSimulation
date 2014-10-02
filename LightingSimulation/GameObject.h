//
// Abstract class for defining game entities
//

#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <d3d11.h>
#include <DirectXMath.h>

#include "Mesh.h"
#include "Material.h"
#include "Lights.h"

using namespace DirectX;

class GameObject
{
public:
	GameObject(Mesh* mesh);
	GameObject(Material* mat);
	GameObject(Mesh* mesh, Material* mat);
	GameObject(Mesh* mesh, Material* mat, LightMaterial* lightMat);
	virtual ~GameObject();

	virtual void Update(float dt);
	virtual void Draw(ID3D11DeviceContext* devCon);

	virtual void SetPosition(XMFLOAT3 newPosition);
	void SetScale(XMFLOAT3 newScale);

	void SetSampler(ID3D11SamplerState* _sampler);
	void SetSRV(ID3D11ShaderResourceView* srv);
	void SetLightMaterial(LightMaterial* _lightMat);
	
	LightMaterial const GetLightMaterial();
	XMFLOAT4X4 const GetWorldMatrix();
protected:
	Mesh* mesh;
	Material* mat;
	LightMaterial* lightMat;

	UINT stride;
	UINT offset;

	ID3D11Buffer* vBuffer;
	ID3D11Buffer* iBuffer;
	ID3D11ShaderResourceView* srv;
	ID3D11SamplerState* sampler;

	XMFLOAT4X4 worldMat;
	XMFLOAT3 position;
	XMFLOAT3 rotation;
	XMFLOAT3 scale;
};

#endif