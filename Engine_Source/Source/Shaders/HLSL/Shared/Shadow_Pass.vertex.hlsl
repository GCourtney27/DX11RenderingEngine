#include <../Deferred_Rendering/Deferred_Rendering.hlsli>	

// Entry Point
// -----------
VS_OUTPUT_SHADOWPASS main(VS_INPUT_SHADOWPASS vs_in)
{
    VS_OUTPUT_SHADOWPASS vs_out;
    vs_out.biTangent = float3(0.0f, 0.0f, 0.0f);
    vs_out.normal = float3(0.0f, 0.0f, 0.0f);
    vs_out.tangent = float3(0.0f, 0.0f, 0.0f);
    vs_out.texCoords = float2(0.0f, 0.0f);
    
    //vs_in.position.x *= 0.5;
    //vs_in.position.y *= 0.5;
    //vs_in.position.z *= 0.5;
    
    float4 worldPos = mul(float4(vs_in.position, 1.0), cbWorld);
    float4x4 wvpLightSpace = mul(mul(cbWorld, dirLight.lightSpaceView), dirLight.lightSpaceProj);
        
    vs_out.sv_position = mul(float4(vs_in.position, 1.0), wvpLightSpace);
    vs_out.fragPos = worldPos.xyz;
    
    return vs_out;
}