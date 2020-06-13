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
		float x;
		float y;
		float z;
		float w;
		XMFLOAT4 Color;
		XVertx4(float x, float y, float z, float w)
		{
			this->x = x;
			this->y = y;
			this->z = z;
			this->w = w;
			Color.x = 1.0f;
			Color.y = 1.0f;
			Color.z = 0.0f;
			Color.w = 0.0f;
		}
		
		
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



	




}
