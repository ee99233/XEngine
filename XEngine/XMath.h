#pragma once
#include <DirectXMath.h>
using namespace DirectX;
struct Matrix
{
	XMFLOAT4X4 WorldtoviewMatrix;
	Matrix()
	{
		WorldtoviewMatrix.m[0][0] = 0.0f;
		WorldtoviewMatrix.m[0][1] = 0.0f;
		WorldtoviewMatrix.m[0][2] = 0.0f;
		WorldtoviewMatrix.m[0][3] = 0.0f;
		WorldtoviewMatrix.m[1][0] = 0.0f;
		WorldtoviewMatrix.m[1][1] = 0.0f;
		WorldtoviewMatrix.m[1][2] = 0.0f;
		WorldtoviewMatrix.m[1][3] = 0.0f;
		WorldtoviewMatrix.m[2][0] = 0.0f;
		WorldtoviewMatrix.m[2][1] = 0.0f;
		WorldtoviewMatrix.m[2][2] = 0.0f;
		WorldtoviewMatrix.m[2][3] = 0.0f;
		WorldtoviewMatrix.m[3][0] = 0.0f;
		WorldtoviewMatrix.m[3][1] = 0.0f;
		WorldtoviewMatrix.m[3][2] = 0.0f;
		WorldtoviewMatrix.m[3][3] = 0.0f;
	}
};

namespace XMath 
{
	struct XVertx4
	{
		XMFLOAT3 Pos;
		XMFLOAT4 Color;
		
	};
	struct  XVertx2
	{

		float u;
		float v;

	};

	struct TriAngle
	{
		XVertx4 vertx[3];
		XVertx4 Norml;
		XVertx2 texture[3];
	};



	static XMFLOAT4X4 Identity4x4()
	{
		static DirectX::XMFLOAT4X4 I(
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);

		return I;
	}





}
