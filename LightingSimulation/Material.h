#ifndef MATERIAL_H
#define MATERIAL_H

#include <d3d11.h>
#include <DirectXMath.h>
#include "Shader.h"
using namespace DirectX;

class Material
{
public:
	Material(wchar_t* vertfilepath, wchar_t* pixelfilepath, ID3D11SamplerState* _sampler, ID3D11Device* dev);
	Material(wchar_t* filepath, ID3D11SamplerState* sampler, ID3D11Device* dev);
	virtual ~Material();

	void LoadShader(Shader* shader);
	void LoadShader(wchar_t* filepath, ShaderType type, ID3D11Device* dev);
	void LoadNormal(wchar_t* filepath, ID3D11Device* dev);
	void LoadBump(wchar_t* filepath, ID3D11Device* dev);

	void SetShader(ID3D11DeviceContext* devCon);
	void SetSampler(ID3D11DeviceContext* devCon);
	void SetResources(ID3D11DeviceContext* devCon);

	ID3D11ShaderResourceView*	GetSRV();
	ID3D11SamplerState*			GetSampler();
	ID3D11ShaderResourceView* srv;
	ID3D11SamplerState* sampler;
private:
	ID3D11ShaderResourceView* normal;
	ID3D11ShaderResourceView* bump;

	Shader* m_Shader;
	ID3D11Buffer* cBuffer;
	

	UINT tileXZ[2];
};

#endif