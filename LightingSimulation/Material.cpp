#include "Material.h"
#include <WICTextureLoader.h>
#include <DDSTextureLoader.h>
#include "Game.h"

Material::Material(wchar_t* filepath, ID3D11SamplerState* sampler, ID3D11Device* dev) :
sampler(sampler)
{
	CreateWICTextureFromFile(
		dev,
		filepath,
		0,
		&srv);
	m_Shader = new Shader();
}

Material::Material(wchar_t* vertfilepath, wchar_t* pixelfilepath, ID3D11SamplerState* _sampler, ID3D11Device* dev)
{
	m_Shader = new Shader();
	m_Shader->LoadShader(vertfilepath, Vert, dev);
	m_Shader->LoadShader(pixelfilepath, Pixel, dev);
	sampler = _sampler;
}

Material::~Material()
{
	ReleaseMacro(srv);
	ReleaseMacro(sampler);
	ReleaseMacro(cBuffer);
	ReleaseMacro(normal);
	ReleaseMacro(bump);
}

ID3D11ShaderResourceView* Material::GetSRV()
{
	return srv;
}

ID3D11SamplerState* Material::GetSampler()
{
	return sampler;
}

void Material::LoadShader(Shader* shader)
{
	m_Shader = shader;
}

void Material::LoadShader(wchar_t* filepath, ShaderType type, ID3D11Device* dev)
{
	m_Shader->LoadShader(filepath, type, dev);
}

void Material::SetShader(ID3D11DeviceContext* devCon)
{
	m_Shader->SetShader(Vert, devCon);
	m_Shader->SetShader(Pixel, devCon);
	m_Shader->SetShader(Geometry, devCon);
	m_Shader->SetShader(Compute, devCon);
	m_Shader->SetShader(Domain, devCon);
}

void Material::LoadNormal(wchar_t* filepath, ID3D11Device* dev)
{
	CreateWICTextureFromFile(
		dev,
		filepath,
		0,
		&normal);
}

void Material::LoadBump(wchar_t* filepath, ID3D11Device* dev)
{
	CreateWICTextureFromFile(
		dev,
		filepath,
		0,
		&bump);
}

void Material::SetSampler(ID3D11DeviceContext* devCon)
{
	if (sampler)
	{
		devCon->VSSetSamplers(0, 1, &sampler);
		devCon->PSSetSamplers(0, 1, &sampler);
	}
}

void Material::SetResources(ID3D11DeviceContext* devCon)
{
	if (srv)
	{
		devCon->VSSetShaderResources(0, 1, &srv);
		devCon->PSSetShaderResources(0, 1, &srv);
	}
	if (normal)
	{
		devCon->VSSetShaderResources(1, 1, &normal);
		devCon->PSSetShaderResources(1, 1, &normal);
	}
	if (bump)
	{
		devCon->VSSetShaderResources(2, 1, &bump);
		devCon->PSSetShaderResources(2, 1, &bump);
	}
}