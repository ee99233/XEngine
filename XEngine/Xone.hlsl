cbuffer cbproject : register(b0)
{
    float4x4 gWorldtoview;
}


void VS(float3 Ipos : POSTION, float4 icolor : COLOR, out float4 opos : SV_Position, out float4 ocolor : COLOR)
{
    opos = mul(float4(Ipos,1.0f), gWorldtoview);
    ocolor = icolor;
}