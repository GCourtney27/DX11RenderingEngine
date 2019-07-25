#pragma once
#include <DirectXMath.h>

struct CB_VS_vertexshader
{
	DirectX::XMMATRIX wvpMatrix;
	DirectX::XMMATRIX worldMatrix;
};

struct CB_VS_vertexshader_2d
{
	DirectX::XMMATRIX wvpMatrix;
};

//struct CB_PS_pixelshader
//{
//	float alpha = 1.0f;
//};

struct CB_PS_light
{
	DirectX::XMFLOAT3 ambientLightColor;//12
	float ambientLightStrength;//4
	//16

	DirectX::XMFLOAT3 dynamicLightColor;//12
	float dynamicLightStrength;//4
	//16
	DirectX::XMFLOAT3 dynamicLightPosition;//12
	float dynamicLightAttenuation_a;
	float dynamicLightAttenuation_b;
	float dynamicLightAttenuation_c;
};