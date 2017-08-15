cbuffer MatrixWVP
{
	float4x4 wvp : WORLDVIEWPROJECTION;
}

RasterizerState DisableCulling
{
	CullMode = NONE;
};

struct VS_INPUT
{
	float4 Pos : POSITION;
	float4 Color : COLOR;
};

struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float4 Color : COLOR;
};


PS_INPUT VS(VS_INPUT input)
{
	PS_INPUT output = (PS_INPUT)0;
	output.Pos = mul(input.Pos, wvp);
	output.Color = input.Color;
	return output;
}

float4 PS(PS_INPUT input) : SV_Target
{
	return input.Color;
}

technique11 main11
{
	pass p0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS()));
		SetRasterizerState(DisableCulling);
	}
}