cbuffer MatrixBuffer:register(b0)
{
	matrix World;
	matrix View;
	matrix Projection;
};

struct VSInput
{
	float4 Pos:POSITION;
	float2 Texcoord:TEXCOORD0;
	float3 Normal:NORMAL;
};

struct PSInput
{
	float4 Pos:SV_POSITION;
	float2 Texcoord:TEXCOORD0;
	float3 Normal:NORMAL;
};

PSInput VSMain(VSInput input)
{
	PSInput output;
	output.Pos = mul(input.Pos, World);
	output.Pos = mul(output.Pos, View);
	output.Pos = mul(output.Pos, Projection);

	output.Texcoord = input.Texcoord;
	output.Normal = mul(input.Normal, (float3x3)World);
	output.Normal = normalize(output.Normal);

	return output;
}