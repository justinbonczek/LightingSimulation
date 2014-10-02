//
// Abstract class for defining game entities
//

#include "GameObject.h"
#include "Game.h"

GameObject::GameObject(Mesh* mesh):
mesh(mesh)
{
	vBuffer = mesh->GetVertexBuffer();
	iBuffer = mesh->GetIndexBuffer();

	stride = sizeof(Vertex);
	offset = 0;

	worldMat = {
		1.0, 0.0, 0.0, 0.0,
		0.0, 1.0, 0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,
		0.0, 0.0, 0.0, 1.0 };
	position = { 0.0, 0.0, 0.0 };
	rotation = { 0.0, 0.0, 0.0 };
	scale = { 1.0, 1.0, 1.0 };
}

GameObject::GameObject(Material* mat) :
mat(mat)
{
	srv = mat->GetSRV();
	sampler = mat->GetSampler();

	stride = sizeof(Vertex);
	offset = 0;

	XMStoreFloat4x4(&worldMat, XMMatrixIdentity());
	position = { 0.0, 0.0, 0.0 };
	rotation = { 0.0, 0.0, 0.0 };
	scale = { 1.0, 1.0, 1.0 };
}

GameObject::GameObject(Mesh* mesh, Material* mat) :
mesh(mesh),
mat(mat)
{
	vBuffer = mesh->GetVertexBuffer();
	iBuffer = mesh->GetIndexBuffer();
	srv = mat->GetSRV();
	sampler = mat->GetSampler();

	stride = sizeof(Vertex);
	offset = 0;

	worldMat = {
		1.0, 0.0, 0.0, 0.0,
		0.0, 1.0, 0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,
		0.0, 0.0, 0.0, 1.0 };
	position = { 0.0, 0.0, 0.0 };
	rotation = { 0.0, 0.0, 0.0 };
	scale = { 1.0, 1.0, 1.0 };
}

GameObject::GameObject(Mesh* mesh, Material* mat, LightMaterial* lightMat)
{
	vBuffer = mesh->GetVertexBuffer();
	iBuffer = mesh->GetIndexBuffer();
	srv = mat->GetSRV();
	sampler = mat->GetSampler();

	stride = sizeof(Vertex);
	offset = 0;

	worldMat = {
		1.0, 0.0, 0.0, 0.0,
		0.0, 1.0, 0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,
		0.0, 0.0, 0.0, 1.0 };
	position = { 0.0, 0.0, 0.0 };
	rotation = { 0.0, 0.0, 0.0 };
	scale = { 1.0, 1.0, 1.0 };
}

GameObject::~GameObject()
{
	ReleaseMacro(vBuffer);
	ReleaseMacro(iBuffer);
	ReleaseMacro(srv);
	ReleaseMacro(sampler);
}

void GameObject::Update(float dt)
{
	worldMat = {
		1.0, 0.0, 0.0, 0.0,
		0.0, 1.0, 0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,
		0.0, 0.0, 0.0, 1.0 };

	// Do the math!
	//XMStoreFloat4x4(&worldMat, XMMatrixMultiply(XMLoadFloat4x4(&worldMat), XMMatrixTranslationFromVector(XMLoadFloat3(&position))));
	worldMat._14 = position.x;
	worldMat._24 = position.y;
	worldMat._34 = position.z;
	worldMat._11 = scale.x;
	worldMat._22 = scale.y;
	worldMat._33 = scale.z;
}

void GameObject::Draw(ID3D11DeviceContext* devCon)
{
	mat->SetShader(devCon);
	mat->SetSampler(devCon);
	mat->SetResources(devCon);
	if (vBuffer)
		devCon->IASetVertexBuffers(0, 1, &vBuffer, &stride, &offset);
	if (iBuffer)
		devCon->IASetIndexBuffer(iBuffer, DXGI_FORMAT_R32_UINT, 0);

	devCon->DrawIndexed(mesh->numIndices, 0, 0);
}

void GameObject::SetPosition(XMFLOAT3 newPosition)
{
	position.x = newPosition.x;
	position.y = newPosition.y;
	position.z = newPosition.z;
}

void GameObject::SetScale(XMFLOAT3 newScale)
{
	scale.x = newScale.x;
	scale.y = newScale.y;
	scale.z = newScale.z;
}

void GameObject::SetSampler(ID3D11SamplerState* _sampler)
{
	sampler = _sampler;
}

void GameObject::SetSRV(ID3D11ShaderResourceView* _srv)
{
	srv = _srv;
}

void GameObject::SetLightMaterial(LightMaterial* _lightMat)
{
	lightMat = _lightMat;
}

LightMaterial const GameObject::GetLightMaterial(){ return *lightMat; }
XMFLOAT4X4 const GameObject::GetWorldMatrix() { return worldMat; }