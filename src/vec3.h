#pragma once

typedef struct vec3{
	double x, y, z;
} vec3;

vec3 make_vec3(double x, double y, double z){
	return (vec3){x, y, z};
}

vec3 negate_vec3(vec3 a){
	return (vec3){-a.x, -a.y, -a.z};
}

vec3 add_vec3(vec3 a, vec3 b){
	return (vec3){a.x+b.x, a.y+b.y, a.z+b.z};
}

vec3 sub_vec3(vec3 a, vec3 b){
	return add_vec3(a,negate_vec3(b));
}

vec3 mul_vec3(double f, vec3 a){
	return (vec3){f*a.x, f*a.y, f*a.z};
}

double length_vec3(vec3 a){
	return sqrt(a.x*a.x + a.y*a.y + a.z*a.z);
}

vec3 normalize_vec3(vec3 a){
	double l = length_vec3(a);
	return (vec3){a.x/l, a.y/l, a.z/l};
}
