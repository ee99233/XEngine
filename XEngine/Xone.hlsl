cbuffer cbproject : register(b0)
{
    float4x4 gWorldtoview;
}


void VS(float4 Ipos:Postion,float4 icolor:COLOR,out float4 opos:SV_Position,out float4 ocolor:COLOR)
{
    opos = mul(Ipos, gWorldtoview);
    ocolor = icolor;
}