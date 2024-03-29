cbuffer MatrixWVP
{
	float4x4 wvp : WORLDVIEWPROJECTION;
}



Texture2D ColorTexture;
Texture2D ColorTextureBg;

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

float4 pixel_shader(VS_OUTPUT IN) : SV_Target
{
	float4 Color = ColorTexture.Sample(ColorSampler, IN.TexCoord) * IN.Color;

	uint2 pos_xy = IN.Position;
	float4 ColorBg = ColorTextureBg[pos_xy];
	
	Color.r = Color.r + Color.r * ColorBg.r / (1.0f - ColorBg.r);
	Color.g = Color.g + Color.g * ColorBg.g / (1.0f - ColorBg.g);
	Color.b = Color.b + Color.b * ColorBg.b / (1.0f - ColorBg.b);
	
	return Color;
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