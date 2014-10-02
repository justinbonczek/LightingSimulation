//
// Class to hold shader pointers for materials. 
// WARNING: DO NOT USE SHADER INSTANCE WITHOUT LOADING SHADERS
//

#ifndef SHADER_H
#define SHADER_H

#include <d3d11.h>

enum ShaderType
{
	Vert,
	Geometry,
	Pixel,
	Compute,
	Domain
};

class Shader
{
public:
	Shader();
	virtual ~Shader();
	
	bool LoadShader(wchar_t* filepath, ShaderType type, ID3D11Device* dev);
	void SetShader(ShaderType type, ID3D11DeviceContext* devCon);

private:
	bool CheckLoaded(ShaderType type);

	ID3D11VertexShader* vert;
	ID3D11PixelShader*  pix;
	ID3D11GeometryShader* geo;
	ID3D11ComputeShader* comp;
	ID3D11DomainShader* dom;
};
#endif