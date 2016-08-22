#version 330

#define BLUR_KERNAL_RADIUS      4
#define BLUR_KERNAL_LEFT        (-BLUR_KERNAL_RADIUS)
#define BLUR_KERNAL_RIGHT       (BLUR_KERNAL_RADIUS)
#define OUTLINE_COLOR           0.8, 1.0, 1.0

in vec2 vTexCoord;

uniform sampler2D uTextureUnit0;
uniform isampler2D uTextureUnit1;

out vec4 colorOutput;

vec4 getBlurredPixel(in ivec3 outline)
{
    vec2 dim = textureSize(uTextureUnit1, 0);

    int outlineCount = 0;
    int sampleCount = 0;

    for (int i = BLUR_KERNAL_LEFT; i < BLUR_KERNAL_RIGHT; i++)
    {
        for (int j = BLUR_KERNAL_LEFT; j < BLUR_KERNAL_RIGHT; j++)
        {
            float x = vTexCoord.x + (i / dim.x);
            float y = vTexCoord.y + (j / dim.y);

            ivec4 value = texture(uTextureUnit1, vec2(x, y));

            if (value.rgb == outline.rgb)
            {
                outlineCount++;
            }

            sampleCount++;
        }
    }

    float nval = outlineCount / float(sampleCount);

    // DEBUG
    // nval = 0.5;

    return vec4(OUTLINE_COLOR, (1.0 - nval) * pow(nval, 0.5));
}


void main()
{
    // get outline buffer color
    vec4 color = texture(uTextureUnit0, vec2(vTexCoord.x, vTexCoord.y));
    ivec4 outline = texture(uTextureUnit1, vec2(vTexCoord.x, vTexCoord.y));

    // if outline is clear, just render
    if (outline.a == 0)
    {
        colorOutput = color;
        return;
    }

    // otherwise blur the outline
    vec4 blur = getBlurredPixel(outline.rgb);
    colorOutput = mix(color, blur, blur.a) + vec4(blur.rgb * blur.a, blur.a);
}
