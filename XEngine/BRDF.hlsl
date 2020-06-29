
#define PI 3.1415926

float GGXNDF(float Rougress,float NOH)
{
    float a = Rougress * Rougress;
    float d = ((NOH * NOH) * (a - 1) + 1) * ((NOH * NOH) * (a - 1) + 1)*PI;
    return 1.0f/d;
}

float3 SchickFrenel(float3 F0,float VOH)
{
    return F0 + (1 - F0) * pow(1 - VOH, 5.0);

}

float G_Smith(float Rougress,float NOL,float NOV)
{
    float k = 8.0f / ((1.0f + Rougress) * (1.0f + Rougress));
    float GL = NOL * (NOL * (1 - k) + k);
    float GV = NOV * (NOV * (1 - k) + k);
    return GL * GV;
}