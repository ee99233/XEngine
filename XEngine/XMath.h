#pragma once
#include <DirectXMath.h>
using namespace DirectX;


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
	};


	struct Matrix
	{
		XMFLOAT4X4 WorldtoviewMatrix = Identity4x4();
		float test = 1.0f;
	};
	struct Matrix1
	{
		float gtest = 1.0f;
		float ftest = 1.0f;
	};

	inline float HillHight(const float &x, const float &y) 
	{

		return 0.3f*(y*sinf(0.1f*x) + x * cosf(y*0.1f));

	}
}
