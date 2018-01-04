#version 330 core

uniform sampler2D cocTex;

in vec2 texCoord;

layout(location = 0) out vec4 cocResult; // 2 channels

void main()
{
#ifdef HORIZONTAL
    const vec2 direction = ivec2(1, 0);
#endif
#ifdef VERTICAL
    const vec2 direction = ivec2(0, 1);
#endif

    ivec2 iTexCoord = ivec2(texCoord * vec2(textureSize(cocTex, 0)));
    cocResult.xy = texelFetch(cocTex, iTexCoord, 0).xy;
    for (int i = -6; i <= 6; i++) {
        ivec2 iCurrCoord = iTexCoord + i * direction;
        vec2 coc = texelFetch(cocTex, iCurrCoord, 0).xy;

        cocResult.x = max(cocResult.x, coc.x);
        cocResult.y = min(cocResult.y, coc.y);
    }
}
