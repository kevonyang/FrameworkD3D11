cbuffer MatrixBuffer:register (b0)
{
	matrix World;
	matrix View;
	matrix Projection;
};

struct VSInput 
{
	float4 Pos:POSITION;
	float2 Texcoord:TEXCOORD0;
};

struct PSInput
{
	float4 Pos:SV_POSITION;
	float2 Texcoord:TEXCOORD0;
};

PSInput VSMain(VSInput input)
{
	PSInput output;
	output.Pos = mul(input.Pos, World);
	output.Pos = mul(output.Pos, View);
	output.Pos = mul(output.Pos, Projection);
	output.Texcoord = input.Texcoord;

	return output;
}