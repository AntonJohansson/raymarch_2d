#pragma once

#include <assert.h>
#include <math.h>

typedef struct vec2{
	double x, y;
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

double dot(vec2 a, vec2 b){
	return a.x*b.x + a.y*b.y;
}

double length(vec2 a){
	return sqrt(dot(a,a));
}

double length2(vec2 a){
	return dot(a,a);
}

vec2 normalize(vec2 a){
	double l = length(a);
	vec2 v = {.x = a.x/l, .y = a.y/l};
	return v;
}

vec2 reflect(vec2 p, vec2 n){
	//assert(length(n) == 1.0);
	double d = dot(p,n);
	return add(p, mul(2.0*d, negate(n)));
}



