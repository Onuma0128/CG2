#include "object3d.hlsli"

struct Material
{
    float4 color;
    int enableLighting;
    float4x4 uvTransform;
    float shininess;
};
struct DirectionalLight
{
    float4 color;
    float3 direction;
    float intensity;
};
struct Camera
{
    float3 worldPosition;
};

ConstantBuffer<Material> gMaterial : register(b0);
ConstantBuffer<DirectionalLight> gDirectionalLight : register(b1);
ConstantBuffer<Camera> gCamera : register(b2);
Texture2D<float4> gTexture : register(t0);
SamplerState gSampler : register(s0);

struct PixelShaderOutput
{
    float4 color : SV_TARGET0;
};

PixelShaderOutput main(VertexShaderOutput input)
{    
    PixelShaderOutput output;
    float4 transformedUV = mul(float4(input.texcoord, 0.0f, 1.0f), gMaterial.uvTransform);
    float4 textureColor = gTexture.Sample(gSampler, transformedUV.xy);
    float3 normal = normalize(input.normal);
    float3 toEye = normalize(gCamera.worldPosition - input.worldPosition);
    float3 lightDir = normalize(gDirectionalLight.direction);
    float3 reflectLight = reflect(-lightDir, normal);
    float RdotE = max(dot(reflectLight, toEye), 0.0);
    float specularPow = pow(RdotE, gMaterial.shininess);
    
    if (gMaterial.enableLighting != 0)
    {
        float NdotL = (dot(normal, lightDir));
         float cos = pow(NdotL * 0.5f + 0.5f, 2.0f);
        float3 diffuse = gMaterial.color.rgb * textureColor.rgb * gDirectionalLight.color.rgb * cos * gDirectionalLight.intensity;
        float3 specular = gDirectionalLight.color.rgb * gDirectionalLight.intensity * specularPow;
        
        output.color.rgb = diffuse + specular;
        output.color.a = gMaterial.color.a * textureColor.a;
    }
    else
    {
        output.color = gMaterial.color * textureColor;
    }

    if (textureColor.a == 0.0f || textureColor.a <= 0.5f || output.color.a == 0.0f)
    {
        discard;
    }
    
    return output;
}