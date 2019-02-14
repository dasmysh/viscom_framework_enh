
// blur algorithm from:
// https://software.intel.com/en-us/blogs/2014/07/15/an-investigation-of-fast-real-time-gpu-based-image-blur-algorithms
const float bilateral_weights[2] = float[] (0.44908, 0.05092);
const float bilateral_offsets[2] = float[] (0.53805, 2.06278);

const float PI = 3.14159265;

vec4 bilateralBlur(vec2 p, vec2 offset, int maskComp, float bilateral_sigma, sampler2D blurTexture)
{
    vec4 result = vec4(0.0f);
    float totalWeight = 0.0f;
    float variance = bilateral_sigma * bilateral_sigma;
    float gaussNormalize = inversesqrt(2.0f * PI * variance);
    float gaussDiv = 0.5f / variance;

    float centerMask = texelFetch(blurTexture, ivec2(p * textureSize(blurTexture, 0).xy), 0)[maskComp];

    for (int i = 0; i < 2; ++i) {
        vec2 texOffset = bilateral_offsets[i] * offset;
        vec4 color0 = texture(blurTexture, p + texOffset);
        vec4 color1 = texture(blurTexture, p - texOffset);
        if (color0[maskComp] > 0.001) color0 /= color0[maskComp];
        if (color1[maskComp] > 0.001) color1 /= color1[maskComp];
        float maskCenterDiff0 = color0[maskComp] - centerMask;
        float maskCenterDiff1 = color1[maskComp] - centerMask;
        float bilateralWeight0 = gaussNormalize * exp(-maskCenterDiff0 * maskCenterDiff0 * gaussDiv);
        float bilateralWeight1 = gaussNormalize * exp(-maskCenterDiff1 * maskCenterDiff1 * gaussDiv);
        result += bilateral_weights[i] * (bilateralWeight0 * color0 + bilateralWeight1 * color1);
        totalWeight += bilateral_weights[i] * (bilateralWeight0 + bilateralWeight1);
    }
    result /= totalWeight;
    result[maskComp] = centerMask;

    return result;
}
