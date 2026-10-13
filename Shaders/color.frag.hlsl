cbuffer Uniforms : register(b0, space3)
{
    float4 color;
}

struct Input
{
    float4 color : TEXCOORD0;
};

struct Output
{
	float4 color : SV_Target0;
};

Output main(Input input)
{
	Output output;
	output.color = color; // float4(0.7f, 0.1f, 0.6f, 1.0f);

	return output;
}
