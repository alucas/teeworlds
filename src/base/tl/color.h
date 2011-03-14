/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */

#ifndef TL_FILE_COLOR_HPP
#define	TL_FILE_COLOR_HPP

#include <base/vmath.h>

inline float HueToRgb(float v1, float v2, float h)
{
   if(h < 0.0f) h += 1;
   if(h > 1.0f) h -= 1;
   if((6.0f * h) < 1.0f) return v1 + (v2 - v1) * 6.0f * h;
   if((2.0f * h) < 1.0f) return v2;
   if((3.0f * h) < 2.0f) return v1 + (v2 - v1) * ((2.0f/3.0f) - h) * 6.0f;
   return v1;
}

inline vec3 HslToRgb(vec3 HSL)
{
	if(HSL.s == 0.0f)
		return vec3(HSL.l, HSL.l, HSL.l);
	else
	{
		float v2 = HSL.l < 0.5f ? HSL.l * (1.0f + HSL.s) : (HSL.l+HSL.s) - (HSL.s*HSL.l);
		float v1 = 2.0f * HSL.l - v2;

		return vec3(HueToRgb(v1, v2, HSL.h + (1.0f/3.0f)), HueToRgb(v1, v2, HSL.h), HueToRgb(v1, v2, HSL.h - (1.0f/3.0f)));
	}
}

inline vec3 HslToRgb(int HslColor)
{
	vec3 r = vec3(
		((HslColor>>16)&0xff)/255.0f,
		((HslColor>>8)&0xff)/255.0f,
		0.5f+(HslColor&0xff)/255.0f*0.5f);
	
	return HslToRgb(r);
}

inline vec4 HslToRgbV4(int HslColor)
{
	vec3 r = HslToRgb(HslColor);

	return vec4(r.r, r.g, r.b, 1.0f);
}

#endif

