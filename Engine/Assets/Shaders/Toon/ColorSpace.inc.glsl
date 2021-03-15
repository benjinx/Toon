#ifndef TOON_COLOR_SPACE_INC_GLSL
#define TOON_COLOR_SPACE_INC_GLSL

// https://en.wikipedia.org/wiki/SRGB

vec4 LinearToSRGB(vec4 color)
{
    return vec4(
        (color.r < 0.0031308 ? 12.92 * color.r : 1.055 * pow(color.r, 1.0 / 2.4) - 0.055),
        (color.g < 0.0031308 ? 12.92 * color.g : 1.055 * pow(color.g, 1.0 / 2.4) - 0.055),
        (color.b < 0.0031308 ? 12.92 * color.b : 1.055 * pow(color.b, 1.0 / 2.4) - 0.055),
        color.a
    );
}

vec4 SRGBToLinear(vec4 color)
{
    return vec4(
	    (color.r < 0.04045 ? (1.0 / 12.92) * color.r : pow((color.r + 0.055) * (1.0 / 1.055), 2.4)),
	    (color.g < 0.04045 ? (1.0 / 12.92) * color.g : pow((color.g + 0.055) * (1.0 / 1.055), 2.4)),
	    (color.b < 0.04045 ? (1.0 / 12.92) * color.b : pow((color.b + 0.055) * (1.0 / 1.055), 2.4)),
        color.a
    );
}

#endif // TOON_COLOR_SPACE_INC_GLSL