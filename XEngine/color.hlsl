//***************************************************************************************
// color.hlsl by Frank Luna (C) 2015 All Rights Reserved.
//
// Transforms and colors geometry.
//***************************************************************************************
#include "Light.hlsl"
cbuffer cbPerObject : register(b0)
{
	float4x4 gWorldViewProj; 
    
};

cbuffer cbLight : register(b1)
{
    
    
};

struct VertexIn
{
	float3 PosL  : POSITION;
    float4 Color : COLOR;
    float3 Nornal : NORMAL;
    
};

struct VertexOut
{
	float4 PosH  : SV_POSITION;
    float4 Color : COLOR;
};

VertexOut VS(VertexIn vin)
{
	VertexOut vout;
	
	// Transform to homogeneous clip space.
    vout.PosH = mul(float4(vin.PosL,1.0f), gWorldViewProj);
	
    
	// Just pass vertex color into the pixel shader.
    vout.Color = vin.Color;
    
    return vout;
}

float4 PS(VertexOut pin) : SV_Target
{
    return pin.Color;
}


