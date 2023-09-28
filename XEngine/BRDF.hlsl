
#define PI 3.1415926

float RadicalInverse_VdC(uint bits)
{
    bits = (bits << 16) | (bits >> 16);
    bits = ((bits & 0x00ff00ff) << 8) | ((bits & 0xff00ff00) >> 8);
    bits = ((bits & 0x0f0f0f0f) << 4) | ((bits & 0xf0f0f0f0) >> 4);
    bits = ((bits & 0x33333333) << 2) | ((bits & 0xcccccccc) >> 2);
    bits = ((bits & 0x55555555) << 1) | ((bits & 0xaaaaaaaa) >> 1);
    return float(bits) * 2.3283064365386963e-10;
                // 0x100000000
}

float2 Hammersley(uint i, uint N)
{
    return float2(float(i) / float(N), RadicalInverse_VdC(i));
}

//float2 Hammersley(uint Index, uint NumSamples, uint2 Random)
//{
//    float E1 = frac((float) Index / NumSamples + float(Random.x & 0xffff) / (1 << 16));
//    float E2 = float(ReverseBits32(Index) ^ Random.y) * 2.3283064365386963e-10;
//    return float2(E1, E2);
//}

float GGXNDF(float Rougress,float NOH)
{
    float a = Rougress * Rougress;
    float d = ((NOH * NOH) * (a - 1) + 1) * ((NOH * NOH) * (a - 1) + 1)*PI;
    return 1.0f/d;
    
}

float3 SchickFrenel(float3 F0,float VOH)
{
    return F0 + (1.0 - F0) * pow(1.0 - VOH, 5.0);

}

float G_Smith(float Rougress,float NOL,float NOV)
{
    float k = 8.0f / ((1.0f + Rougress) * (1.0f + Rougress));
    float GL = NOL * (NOL * (1 - k) + k);
    float GV = NOV * (NOV * (1 - k) + k);

    return GL * GV;
    
}


float3 ImportGGX(float Rougress,float3 N,float2 Xi)
{
    float a = Rougress * Rougress;
    float Phi = 2 * PI * Xi.x;
    float CosTh = sqrt((1 - Xi.y) / ((a - 1) * Xi.y + 1));
    float SintH = sqrt(1 - CosTh * CosTh);
    
    float3 h;
    h.x = SintH * cos(Phi);
    h.y = SintH * sin(Phi);
    h.z = CosTh;
    
    float3 tz = abs(N.z) < 0.999 ? float3(0.f, 0.f, 1.f) : float3(1.0f, 0.0f, 0.0f);
    float3 tx = normalize(cross(tz, N));
    float3 ty = cross(N, tx);
    return tx * h.x + ty * h.y + N * h.z;
    

}



float3 SpecularIBL(float3 SpecularColor,float Rougress, float3 N,float3 V)
{
    
    int NunSample = 1024;
    float3 SumColor;
    for (int i = 0; i < NunSample;++i)
    {
        
        float2 XI = Hammersley(i, NunSample);
        float3 H = ImportGGX(3, N, XI);
        float3 L = 2 * dot(V, H) * H - V;
        float NOL = saturate(dot(N, L));
        float VOH = saturate(dot(V, H));
        float NOV = saturate(dot(N, V));
        float NOH = saturate(dot(N, H));
        if (NOL>0.0f)
        {
            float G = G_Smith(Rougress,NOL,NOV);
            float FC = pow(1 - VOH, 5.0);
            float3 F = FC + SpecularColor * (1 - FC); 
            SumColor += (G * F * VOH) / (NOV * NOH);
            
        }
       
    }
    return SumColor / NunSample;
    
}

