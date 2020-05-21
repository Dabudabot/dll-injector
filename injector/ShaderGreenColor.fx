// ShaderGreenColor.fx : Implements the shader.
//
// By Geelix School of Serious Games and Edutainment.
//


float4 VS_Main(float4 pos : POSITION) : SV_POSITION
{
    return pos;
}

float4 PS_Main(float4 pos : SV_POSITION) : SV_TARGET
{
    return float4(0.0f, 0.75f, 0.0f, 1.0f);
}
