//
// A basic 3d camera class 
// Credit to Frank D. Luna for reference
//

#ifndef CAMERA_H
#define CAMERA_H

#include <d3d11.h>
#include <DirectXMath.h>
#include "Shader.h"
#include "Mesh.h"

using namespace DirectX;

class Camera
{
public:
	Camera();
	~Camera();

	XMVECTOR GetPositionXM() const;
	XMFLOAT3 GetPosition()   const;
	void	 SetPosition(float x, float y, float z);
	void	 SetPosition(XMFLOAT3& v);

	XMVECTOR GetRightXM() const;
	XMFLOAT3 GetRight()   const;
	XMVECTOR GetUpXM() const;
	XMFLOAT3 GetUp()   const;
	XMVECTOR GetLookXM() const;
	XMFLOAT3 GetLook()   const;

	float GetNearZ()  const;
	float GetFarZ()   const;
	float GetAspect() const;
	float GetFovX()   const;
	float GetFovY()   const;

	float GetNearWindowWidth() const;
	float GetNearWindowHeight() const;
	float GetFarWindowWidth() const;
	float GetFarWindowHeight() const;

	void SetLens(float fovY, float aspect, float zn, float zf);

	void LookAt(FXMVECTOR pos, FXMVECTOR target, FXMVECTOR worldUp);
	void LookAt(const XMFLOAT3& pos, const XMFLOAT3& target, const XMFLOAT3& up);

	XMMATRIX View() const;
	XMMATRIX Proj() const;
	XMMATRIX ViewProj() const;

	void Strafe(float d);
	void Walk(float d);

	void Pitch(float angle);
	void RotateY(float angle);

	void UpdateViewMatrix();
private:
	XMFLOAT3 m_Position;
	XMFLOAT3 m_Right;
	XMFLOAT3 m_Up;
	XMFLOAT3 m_Look;

	UINT stride;
	UINT offset;

	float m_NearZ;
	float m_FarZ;
	float m_Aspect;
	float m_FovX;
	float m_FovY;
	float m_NearWindowHeight;
	float m_FarWindowHeight;
	float m_NearWindowWidth;
	float m_FarWindowWidth;
	
	XMFLOAT4X4 m_View;
	XMFLOAT4X4 m_Proj;
};

#endif