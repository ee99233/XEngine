#include "Light.hlsl"
#include "BRDF.hlsl"
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
    Light light[3];
    float albedo;
    Xmartail mat;
    
}

struct VertexIn
{
    float3 PosL : POSITION;
    float4 Color : COLOR;
    float3 NORMAL : NORMAL;
};

struct VertexOut
{
    float4 PosH : SV_POSITION;
    float4 Color : COLOR;
    float3 PosW : POSITIONT;
    float3 Normal : NORMAL;
};



VertexOut VS(VertexIn pin)
{
    VertexOut vout;
	
    vout.PosH = mul(float4(pin.PosL, 1.0f), gWorldtoview);
    vout.PosW = mul(float4(pin.PosL, 1.0f),gworld);
    vout.Normal = mul(pin.NORMAL, (float3x3) gworld);
    vout.Color = pin.Color;
    return vout;

}

float4 PS(VertexOut pout) : SV_Target
{
    float3 V = normalize(Campos - pout.PosW);
    float N = normalize(pout.Normal);
    float3 F0 = 0.04;
    F0 = lerp(F0, mat.BaseColor, mat.metallic);
    float3 Sumcolor;
    for (int i = 0; i < 3;i++)
    {
        
        float3 L = normalize(light[i].LoghtPos - pout.PosW);
        float Llength = length(light[i].LoghtPos - pout.PosW);
        float atten = 1.0f / (Llength * Llength);
        float3 H = normalize(V + L);
        float NOL = saturate(dot(N, L));
        float VOH = saturate(dot(V, H));
        float NOH = saturate(dot(N, H));
        float NOV = saturate(dot(N, V));
        if (NOL>0.0f)
        {
            float kd = (1 - F0) * (1 - mat.metallic);
            float D = GGXNDF(mat.Rougress, NOH);
            float F = SchickFrenel(F0, VOH);
            float G = G_Smith(mat.Rougress,NOL, NOV);
            float num = (4 * NOV * NOL);
            float3 specular = (D * F * G) / max(num,0.001);
            Sumcolor += (specular + kd * (mat.BaseColor / PI)) * light[i].LightColor*atten*NOL;
        }
    }
    
    
    float3 ambient = float3(3.0f) * ao * albedo;
    float3 color=Sumcolor + ambient;
    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0 / 2.2));
    
    return pout.Color;
}

