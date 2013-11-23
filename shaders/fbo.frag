#version 330

#extension GL_EXT_gpu_shader4 : enable

#define BLUR_KERNAL_SIZE        6
#define BLUR_KERNAL_STRIDE      1

#define OUTLINE_COLOUR          1.0, 1.0, 1.0

in vec2 texCoordOut;

uniform sampler2D uTextureUnit0;
uniform sampler2D uTextureUnit1;
uniform sampler2D uTextureUnit2;

out vec4 colour;


vec4 getBlurredPixel()
{
    vec2 dim = textureSize2D( uTextureUnit1, 0 );
    
    float outlineCount = 0;
    int   sampleCount  = 0;
    
    for ( int i= -( BLUR_KERNAL_SIZE-1 ); i < BLUR_KERNAL_SIZE; i+=BLUR_KERNAL_STRIDE )
    {
        for ( int j= -( BLUR_KERNAL_SIZE-1 ); j < BLUR_KERNAL_SIZE; j+=BLUR_KERNAL_STRIDE )
        {
            float x = texCoordOut.x + ( i / dim.x );
            float y = texCoordOut.y + ( j / dim.y );
            outlineCount += texture2D( uTextureUnit1, vec2( x, y ) ).r;
            sampleCount++;
        }
    }
    
    float x = outlineCount / float( sampleCount );
         
    return vec4( OUTLINE_COLOUR, pow( x, 0.5 ) );
}


void main()
{   
    // get outline buffer colour
    vec4 opaque  = texture2D( uTextureUnit0, vec2( texCoordOut.x, texCoordOut.y ) );
    vec4 outline = texture2D( uTextureUnit1, vec2( texCoordOut.x, texCoordOut.y ) );
    vec4 arthro  = texture2D( uTextureUnit2, vec2( texCoordOut.x, texCoordOut.y ) );
    
    // if it is not clear, render normal buffer only    
    if ( outline.r > 0 )
    {
        colour = mix( arthro, opaque, opaque.a );
        return;
    }

    vec4 temp = mix( arthro, opaque, opaque.a );
    vec4 blur = getBlurredPixel();
    colour = mix( temp, blur, blur.a );

    /*
    vec4 blur = getBlurredPixel();
    if ( opaque.a == 0 )
    {
        colour = blur + vec4( (1 - blur.a)*arthro.rgb, arthro.a );  //blur;
        return;
    }

    // otherwise, render blurred edge outline
    colour =  opaque + vec4 (blur.rgb* blur.a, 0.0 ) + vec4( (1 - blur.a)*arthro.rgb, arthro.a ); // opaque + vec4 (blur.rgb* blur.a, 0.0 );
    */
}
