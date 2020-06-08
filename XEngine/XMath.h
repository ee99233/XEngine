#pragma once
namespace XMath 
{
	struct XVertx4
	{
		float x;
		float y;
		float z;
		float w;
		
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
