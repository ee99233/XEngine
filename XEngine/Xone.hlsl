#include "Light.hlsl"
#include "BRDF.hlsl"

Texture2D BrickMap : register(t0);


SamplerState gsamPointWrap : register(s0);
SamplerState gsamPointClamp : register(s1);
SamplerState gsamLinearWrap : register(s2);
SamplerState gsamLinearClamp : register(s3);
SamplerState gsamAnisotropicWrap : register(s4);
SamplerState gsamAnisotropicClamp : register(s5);

cbuffer cbproject : register(b0)
{
    float4x4 gWorldtoview;
    float test;
}

cbuffer cbtest: register(b1)
{
    float4x4 gworld;
    float3 Campos;
    float ao;
    Xmartail mat;
    float albedo;
    float3 pad;
    Light light[3];
   
    
}



struct VertexIn
{
    float3 PosL : POSITION;
    float4 Color : COLOR;
    float3 NORMAL : NORMAL;
    float2 TextCord : TEXCOORD;
};

struct VertexOut
{
    float4 PosH : SV_POSITION;
    float4 Color : COLOR;
    float4 PosW : POSITIONT;
    float3 Normal : NORMAL;
    float2 TextCord : TEXCOORD;
};



VertexOut VS(VertexIn pin)
{
    VertexOut vout;
	
    vout.PosH = mul(float4(pin.PosL, 1.0f), gWorldtoview);
    vout.PosW = mul(float4(pin.PosL, 1.0f),gworld);
    vout.Normal = mul(pin.NORMAL, (float3x3) gworld);
    vout.TextCord = pin.TextCord;
    vout.Color = pin.Color;
    return vout;

}

float4 PS(VertexOut pout) : SV_Target
{
    float4 colorc = BrickMap.Sample(gsamAnisotropicClamp, pout.TextCord);
    float3 co;
    co = float3(1.0f, 0.f, 0.f);
    float3 V = normalize(Campos - (float3) pout.PosW);
    float3 N = normalize(pout.Normal);
    float3 F0 = 0.08;
    F0 = lerp(F0, co, mat.metallic);
    float3 Sumcolor = 0.0f;
    for (int i = 0; i < 3; i++)
    {
        
        float3 L = normalize(light[i].LoghtPos - (float3) pout.PosW);
        float Llength = length(light[i].LoghtPos - (float3) pout.PosW);
       
        float atten = 1.0f / (Llength * Llength);
        
           
        float3 H = normalize(V + L);
        float NOL = saturate(dot(N, L));
        float VOH = saturate(dot(V, H));
        float NOH = saturate(dot(N, H));
        float NOV = saturate(dot(N, V));
        if (NOL > 0.0f)
        {
            float3 kd = (1 - F0) * (1 - mat.metallic);
            float D = GGXNDF(mat.Rougress, NOH);
            float3 F = SchickFrenel(F0, VOH);
            float G = G_Smith(mat.Rougress, NOL, NOV);
            float num = (4 * NOV * NOL);
            float3 specular = (D * F * G) / max(num, 0.001);
            Sumcolor += (specular + kd * (co / 3.14159265359)) * light[i].LightColor * atten * NOL;
        }
    }
    
    Sumcolor = Sumcolor;
    float3 ambient = float3(1.0f, 1.0f, 1.0f) * co;
    float3 color = Sumcolor + ambient;
    color = color / (color + float3(1.0, 1.0, 1.0));
    color = pow(color, float3(1.0f / 2.2f, 1.0f / 2.2f, 1.0f / 2.2f));
    pout.Color = float4(color, 1.0f);
    
    return pout.Color;
}

