#ifndef LIGHTS_H
#define LIGHTS_H

#include <d3d11.h>
#include <DirectXMath.h>
using namespace DirectX;

struct DirectionalLight
{
	DirectionalLight() { ZeroMemory(this, sizeof(this)); }
	XMFLOAT4 ambient;
	XMFLOAT4 diffuse;
	XMFLOAT4 specular;
	XMFLOAT3 direction;
	float pad;
};

struct PointLight
{
	PointLight() { ZeroMemory(this, sizeof(this)); }
	XMFLOAT4 ambient;
	XMFLOAT4 diffuse;
	XMFLOAT4 specular;
	XMFLOAT3 position;
	float range;
	XMFLOAT3 attenuation;
	float pad;
};

struct SpotLight
{
	SpotLight() { ZeroMemory(this, sizeof(this)); }
	XMFLOAT4 ambient;
	XMFLOAT4 diffuse;
	XMFLOAT4 specular;
	XMFLOAT3 position;
	float range;
	XMFLOAT3 direction;
	float spot;
	XMFLOAT3 attenuation;
	float pad;
};

struct LightMaterial
{
	XMFLOAT4 ambient;
	XMFLOAT4 diffuse;
	XMFLOAT4 specular;
	XMFLOAT4 reflect;
};

#endif