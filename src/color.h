#pragma once

#include "vec3.h"

//#define RED 				0xAB4252FF
//#define PINK 				0xF2A3B1FF
//#define PEACH 			0xF9C099FF
//#define GREEN 			0x909700FF
//#define DARK_GREEN 	0x444B09FF
#define RED 				0xFF5242AB
#define PINK 				0xFFB1A3F2
#define PEACH 			0xFF99C0F9
#define GREEN 			0xFF009790
#define DARK_GREEN 	0xFF094B44

#define BACKGROUND 	0xFFD7EBFA

#define HEX_TO_RGBA_COLOR(hex) \
	(hex >> 0) & 0xFF,\
	(hex >> 8) & 0xFF,\
	(hex >> 16) & 0xFF,\
	(hex >> 24) & 0xFF 

double hsl_f(vec3 hsl, int n){
	double k = fmod((n + hsl.x/30), 12);
	double a = hsl.y*fmin(hsl.z, 1 - hsl.z);
	return hsl.z - a*fmax(fmin(fmin(k-3, 9-k),1),-1);
}

vec3 hsl_to_rgb(vec3 hsl){
	return (vec3){hsl_f(hsl, 0), hsl_f(hsl, 8), hsl_f(hsl, 4)};
}


