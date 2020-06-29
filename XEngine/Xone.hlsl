cbuffer cbproject : register(b0)
{
    float4x4 gWorldtoview;
    float test;
}

cbuffer cbtest: register(b1)
{
    float gtest;
    float ftest;
    
}


void VS(float3 Ipos : POSITION, float4 icolor : COLOR, out float4 opos : SV_POSITION, out float4 ocolor : COLOR)
{
    opos = mul(float4(Ipos,1.0f), gWorldtoview);
    float cpos = gtest * 1.0f;
    ocolor = icolor;
}