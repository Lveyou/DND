cbuffer MatrixWVP
{
	float4x4 wvp : WORLDVIEWPROJECTION;
	float dnd_time;
}



Texture2D ColorTexture;
Texture2D NoiseTexture;

SamplerState ColorSampler
{
	Filter = MIN_MAG_MIP_POINT;
	AddressU = WRAP;
	AddressV = WRAP;
};

RasterizerState DisableCulling
{
	CullMode = NONE;
};

struct VS_INPUT
{
	float4 Position : POSITION;
	float4 Color : COLOR;
	float2 TexCoord : TEXCOORD;
};

struct VS_OUTPUT
{
	float4 Position : SV_Position;
	float4 Color : COLOR;
	float2 TexCoord : TEXCOORD;
};

VS_OUTPUT vertex_shader(VS_INPUT IN)
{
	VS_OUTPUT OUT = (VS_OUTPUT)0;
	OUT.Position = mul(IN.Position, wvp);
	OUT.Color = IN.Color;
	OUT.TexCoord = IN.TexCoord;
	
	return OUT;
}


float waveHeight(float2 p){

    float timeFactor=0.79;

    float texFactor=12.0;

    float ampFactor=0.017;

    float dist=length(p);

    return cos(dnd_time*timeFactor+dist*texFactor)*ampFactor;

}


float4 pixel_shader(VS_OUTPUT IN) : SV_Target
{
//1
	float2 c = NoiseTexture.Sample(ColorSampler, IN.TexCoord - float2(0,dnd_time * 0.035)).gb + NoiseTexture.Sample(ColorSampler, IN.TexCoord - float2(dnd_time * 0.014,0)).gb - 1;
	IN.TexCoord += c * 0.07;

//2	
	float2 p = -1.0+2.0*IN.TexCoord;
    float2 normal = normalize(p);
    IN.TexCoord += normal*waveHeight(p);
	
	return ColorTexture.Sample(ColorSampler, IN.TexCoord) * IN.Color;
}


				


technique11 main11
{
	pass p0
	{
		SetVertexShader(CompileShader(vs_5_0, vertex_shader()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, pixel_shader()));

		SetRasterizerState(DisableCulling);
	}
}