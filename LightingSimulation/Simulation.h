///
// Environment Simulation made by Justin Bonczek using DirectX 11
///

#ifndef SIMULATION_H
#define SIMULATION_H

#include <vector>

#include "Game.h"
#include "Camera.h"
#include "GameObject.h"
#include "MeshGenerator.h"
#include "Lights.h"

struct PerFrameData
{
	DirectionalLight dLight;
	PointLight pLight;
	SpotLight sLight;
	XMFLOAT4X4 view;
	XMFLOAT4X4 projection;
	XMFLOAT3 eyePos;
	float time;
};

struct PerObjectData
{
	XMFLOAT4X4 world;
	XMFLOAT4X4 worldInverseTranspose;
	LightMaterial lightMat;
};

class Simulation : public Game
{
public:
	Simulation(HINSTANCE hInstance);
	virtual ~Simulation();
	// Sets up the game
	bool Initialize();

	// Called every time the window is resized
	void OnResize();

	// Called many times per second to update simulation logic
	void Update(float dt);

	// Called many times per second to draw the simulation
	void Draw();
private:
	// Functions called during initialization
	void LoadAssets();
	void InitializePipeline();	
	void MoveCamera(float dt);

	Camera m_Camera;

	PerFrameData perFrameData;
	PerObjectData perObjectData;

	ID3D11Buffer* perFrameBuffer;
	ID3D11Buffer* perObjectBuffer;

	DirectionalLight dLight;
	PointLight pLight;
	SpotLight sLight;

	ID3D11InputLayout* inputLayout;
	
	ID3D11BlendState* blendState;
	ID3D11DepthStencilState* depthStencilState;

	ID3D11RasterizerState* solid;
	ID3D11RasterizerState* wireframe;
	ID3D11RasterizerState* current;
	float totalTime;
	float time;

	std::vector<GameObject*> objects;
};

#endif