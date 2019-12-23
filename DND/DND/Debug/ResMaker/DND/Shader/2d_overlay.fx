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
	

	if (ColorBg.r <= 0.5f)
	{
		Color.r = Color.r * ColorBg.r * 2.0f;
	}
	else
	{
		Color.r = 1.0f - (1.0f - Color.r) * (1.0f - ColorBg.r) * 2.0f;
	}
	if (ColorBg.g <= 0.5f)
	{
		Color.g = Color.g * ColorBg.g * 2.0f;
	}
	else
	{
		Color.g = 1.0f - (1.0f - Color.g) * (1.0f - ColorBg.g) * 2.0f;
	}
	if (ColorBg.b <= 0.5f)
	{
		Color.b = Color.b * ColorBg.b * 2.0f;
	}
	else
	{
		Color.b = 1.0f - (1.0f - Color.b) * (1.0f - ColorBg.b) * 2.0f;
	}

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