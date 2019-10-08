struct PixelInputType {
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

static float2 positions[3] = {
    float2(0.0, -0.5),
    float2(0.5, 0.5),
    float2(-0.5, 0.5)
};

static float3 colors[3] = {
    float3(1.0, 0.0, 0.0),
    float3(0.0, 1.0, 0.0),
    float3(0.0, 0.0, 1.0)
};

PixelInputType VSmain(uint vertexIndex: SV_VertexID) {
    PixelInputType output;
    output.position = float4(positions[vertexIndex], 0.0, 1.0);
    output.color = float4(colors[vertexIndex], 1.0);
    return output;
};

float4 PSmain(PixelInputType input) : SV_TARGET {
    return input.color;
};