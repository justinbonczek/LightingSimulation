#include "Camera.h"

Camera::Camera():
m_View(),
offset(0),
stride(sizeof(Vertex))
{
	XMVECTOR position = XMVectorSet(0, 0, -5, 0);
	XMVECTOR target = XMVectorSet(0, 0, 0, 0);
	XMVECTOR up = XMVectorSet(0, 1, 0, 0);
	XMMATRIX V = XMMatrixLookAtLH(position, target, up);
	XMStoreFloat4x4(&m_View, XMMatrixTranspose(V));

	m_Right = XMFLOAT3(1.0, 0.0, 0.0);
	m_Up = XMFLOAT3(0.0, 1.0, 0.0);
	m_Look = XMFLOAT3(0.0, 0.0, 1.0);
}

Camera::~Camera()
{

}

XMVECTOR Camera::GetPositionXM()const
{
	return XMLoadFloat3(&m_Position);
}

XMFLOAT3 Camera::GetPosition()const
{
	return m_Position;
}

void Camera::SetPosition(float x, float y, float z)
{
	m_Position.x = x;
	m_Position.y = y;
	m_Position.z = z;
}

void Camera::SetPosition(XMFLOAT3& v)
{
	m_Position = v;
}

XMVECTOR Camera::GetRightXM()const
{
	return XMLoadFloat3(&m_Right);
}

XMFLOAT3 Camera::GetRight()const
{
	return m_Right;
}

XMVECTOR Camera::GetUpXM()const
{
	return XMLoadFloat3(&m_Up);
}

XMFLOAT3 Camera::GetUp()const
{
	return m_Up;
}

XMVECTOR Camera::GetLookXM()const
{
	return XMLoadFloat3(&m_Look);
}

XMFLOAT3 Camera::GetLook()const
{
	return m_Look;
}

float Camera::GetNearZ()const
{
	return m_NearZ;
}
float Camera::GetFarZ()const
{
	return m_FarZ;
}
float Camera::GetAspect()const
{
	return m_Aspect;
}
float Camera::GetFovY()const
{
	return m_FovY;
}
float Camera::GetNearWindowHeight()const
{
	return m_NearWindowHeight;
}
float Camera::GetFarWindowHeight()const
{
	return m_FarWindowHeight;
}

void Camera::SetLens(float fovY, float aspect, float zn, float zf)
{
	m_FovY = fovY;
	m_Aspect = aspect;

	m_NearZ = zn;
	m_FarZ = zf;

	m_NearWindowHeight = 2.0f * m_NearZ * tanf(0.5f * m_FovY);
	m_FarWindowHeight = 2.0f * m_FarZ * tanf(0.5f * m_FovY);

	XMMATRIX P = XMMatrixPerspectiveFovLH(m_FovY, m_Aspect, m_NearZ, m_FarZ);
	XMStoreFloat4x4(&m_Proj, P);
}

void Camera::LookAt(FXMVECTOR pos, FXMVECTOR target, FXMVECTOR worldUp){
}

void Camera::LookAt(const XMFLOAT3& pos, const XMFLOAT3& target, const XMFLOAT3& worldUp){

}

float Camera::GetFovX()const{
	float halfWidth = 0.5f * GetNearWindowWidth();
	return 2.0f * atan(halfWidth / m_NearZ);
}

float Camera::GetNearWindowWidth()const{
	return m_Aspect * m_NearWindowHeight;
}
float Camera::GetFarWindowWidth()const{
	return m_Aspect * m_FarWindowHeight;
}

void Camera::Walk(float d)
{
	XMVECTOR s = XMVectorReplicate(d);
	XMVECTOR l = XMLoadFloat3(&m_Look);
	XMVECTOR p = XMLoadFloat3(&m_Position);
	XMStoreFloat3(&m_Position, XMVectorMultiplyAdd(s, l, p));
}

void Camera::Strafe(float d)
{
	XMVECTOR s = XMVectorReplicate(d);
	XMVECTOR r = XMLoadFloat3(&m_Right);
	XMVECTOR p = XMLoadFloat3(&m_Position);
	XMStoreFloat3(&m_Position, XMVectorMultiplyAdd(s, r, p));
}

void Camera::Pitch(float angle)
{
	XMMATRIX R = XMMatrixRotationAxis(XMLoadFloat3(&m_Right), angle);

	XMStoreFloat3(&m_Up, XMVector3TransformNormal(XMLoadFloat3(&m_Up), R));
	XMStoreFloat3(&m_Look, XMVector3TransformNormal(XMLoadFloat3(&m_Look), R));
}

void Camera::RotateY(float angle)
{
	XMMATRIX R = XMMatrixRotationY(angle);

	XMStoreFloat3(&m_Right, XMVector3TransformNormal(XMLoadFloat3(&m_Right), R));
	XMStoreFloat3(&m_Up, XMVector3TransformNormal(XMLoadFloat3(&m_Up), R));
	XMStoreFloat3(&m_Look, XMVector3TransformNormal(XMLoadFloat3(&m_Look), R));
}

void Camera::UpdateViewMatrix()
{
	XMVECTOR R = XMLoadFloat3(&m_Right);
	XMVECTOR U = XMLoadFloat3(&m_Up);
	XMVECTOR L = XMLoadFloat3(&m_Look);
	XMVECTOR P = XMLoadFloat3(&m_Position);

	L = XMVector3Normalize(L);
	U = XMVector3Normalize(XMVector3Cross(L, R));
	R = XMVector3Cross(U, L);
	float x = -XMVectorGetX(XMVector3Dot(P, R));
	float y = -XMVectorGetX(XMVector3Dot(P, U));
	float z = -XMVectorGetX(XMVector3Dot(P, L));

	XMStoreFloat3(&m_Right, R);
	XMStoreFloat3(&m_Up, U);
	XMStoreFloat3(&m_Look, L);

	m_View(0, 0) = m_Right.x;
	m_View(1, 0) = m_Right.y;
	m_View(2, 0) = m_Right.z;
	m_View(3, 0) = x;
	m_View(0, 1) = m_Up.x;
	m_View(1, 1) = m_Up.y;
	m_View(2, 1) = m_Up.z;
	m_View(3, 1) = y;
	m_View(0, 2) = m_Look.x;
	m_View(1, 2) = m_Look.y;
	m_View(2, 2) = m_Look.z;
	m_View(3, 2) = z;
	m_View(0, 3) = 0.0f;
	m_View(1, 3) = 0.0f;
	m_View(2, 3) = 0.0f;
	m_View(3, 3) = 1.0f;
}

XMMATRIX Camera::View()const
{
	return XMLoadFloat4x4(&m_View);
}

XMMATRIX Camera::Proj()const
{
	return XMLoadFloat4x4(&m_Proj);
}