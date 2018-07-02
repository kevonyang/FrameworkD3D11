cbuffer MatrixBuffer:register (b0)
{
	matrix World;
	matrix View;
	matrix Projection;
};

struct VSInput
{
	float4 Position:POSITION;
	float4 Color:COLOR;
};

struct PSInput
{
	float4 Position:SV_POSITION;
	float4 Color:COLOR;
};

PSInput VSMain(VSInput input)
{
	PSInput output;
	output.Position = mul(input.Position, World);
	output.Position = mul(output.Position, View);
	output.Position = mul(output.Position, Projection);
	output.Color = input.Color;

	return output;
}