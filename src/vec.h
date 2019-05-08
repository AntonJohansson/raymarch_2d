#pragma once

#include <stdint.h>

typedef struct vec{
	double* 
	uint8_t dim;
} vec2;

vec2 make_vec2(double x, double y){
	return (vec2){x, y};
}

vec2 negate(vec2 a){
	vec2 v = {.x = -a.x, .y = -a.y};
	return v;
}

vec2 add(vec2 a, vec2 b){
	vec2 v = {.x = a.x + b.x, .y = a.y + b.y};
	return v;
}

vec2 sub(vec2 a, vec2 b){
	return add(a,negate(b));
}

vec2 mul(double f, vec2 a){
	vec2 v = {.x = f*a.x, .y = f*a.y};
	return v;
}

double length(vec2 a){
	return sqrt(a.x*a.x + a.y*a.y);
}

vec2 normalize(vec2 a){
	double l = length(a);
	vec2 v = {.x = a.x/l, .y = a.y/l};
	return v;
}



