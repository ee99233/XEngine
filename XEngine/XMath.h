#pragma once
#include <DirectXMath.h>
#include "StaticMesh.h"
#include "Light.h"
using namespace DirectX;


namespace XMath
{
	struct XVertx4
	{
		XMFLOAT3 Pos;
		XMFLOAT4 Color;
		XMFLOAT3 Normal;
		XMFLOAT2 TextCord;
		XVertx4() 
		{

		}
		XVertx4(float x, float y, float z,float nx, float ny, float nz, float u, float v) :
			Pos(x, y, z),
			Normal(nx, ny, nz),
			TextCord(u, v) {}
		XVertx4(XMFLOAT3 tespos, XMFLOAT4 tesColor, XMFLOAT3 tesNormal, XMFLOAT2 tesTextCord) :
			Pos(tespos),
			Color(tesColor),
			Normal(tesNormal),
			TextCord(tesTextCord) {}
		XVertx4(XMFLOAT3 tespos, XMFLOAT4 tesColor) :
			Pos(tespos),
			Color(tesColor) {}

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
		XMFLOAT4X4 gworld;
		XMFLOAT3 Campos;
		float ao;
		XMaterail mat;
		float albedo;
		XMFLOAT3 pad;
		LightS light[3];
		
	};

	inline float HillHight(const float &x, const float &y)
	{

		return 0.3f*(y*sinf(0.1f*x) + x * cosf(y*0.1f));

	}
	inline XMFLOAT3 CrossTS( XMFLOAT3 &v1, XMFLOAT3 &v2)
	{
		return XMFLOAT3(v1.y*v2.z - v1.z*v2.y, v1.z*v2.x - v1.x*v2.z, v1.x*v2.y - v2.x * v2.y);
	}

	template<typename T>
	static T Clamp(const T& x, const T& low, const T& high)
	{
		return x < low ? low : (x > high ? high : x);
	}

}
