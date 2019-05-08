#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <limits.h>
#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include "perlin.h"
#include "vec2.h"
#include "vec3.h"
#include "color.h"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

bool draw_debug_rays = false;
bool draw_debug_circles = false;
bool draw_debug_ray_circles = false;
int raycount = 0;
int light_intensity = 0;

static SDL_Renderer* renderer = NULL;

typedef struct circle{
	vec2 position;
	Sint16 radius;
} circle;

#define NUMBER_OF_CIRCLES 16
static circle circles[NUMBER_OF_CIRCLES];

#define MAX_RAYCOUNT 1000

typedef struct raymarch_results {
	vec2 cast_direction;
	vec2 start_pos;
	vec2 final_pos;
	vec2 collision_object;
	vec2 object_normal;
	vec3 object_color;
	bool collision_found;
	int object_id;
} raymarch_results;

static raymarch_results raymarch_points[MAX_RAYCOUNT];

double angle = 0.0;

static vec2 position = {
	.x = SCREEN_WIDTH/2,
	.y = SCREEN_HEIGHT/2,
};
static vec2 mouse = {
	.x = 1.1*SCREEN_WIDTH/2,
	.y = 1.0*SCREEN_HEIGHT/2,
};
static vec2 light_pos = (vec2){0,0};




























double bound(double lower, double upper, double value){
	return fmin(fmax(lower, value), upper);
}

double wrap(double lower, double upper, double value){
	if(value < lower){
		return upper;
	}else if(value > upper){
		return lower;
	}else{
		return value;
	}
}




double signed_distance_circle(vec2 p0, vec2 p1, Sint16 r){
	return length(sub(p1,p0))-r;
}

vec2 circle_normal(vec2 circle_position, vec2 collision_position){
	return normalize(sub(collision_position, circle_position));
}

vec3 circle_color(size_t i){
	//return hsl_to_rgb((HSL){i*360.0/(NUMBER_OF_CIRCLES), 0.35, 0.75});
	return hsl_to_rgb((vec3){i*360.0/(NUMBER_OF_CIRCLES), 0.75, 0.5});
}

void draw_vec2(vec2 p, vec2 v){
	lineColor(renderer, p.x, p.y, p.x+10.0*v.x, p.y+10.0*v.y, 0xFF0000FF);
}

raymarch_results raymarch(vec2 position, vec2 direction){
	vec2 start_position = position;
	position = add(position, mul(0.1, direction));

	direction = normalize(direction);

	double min_distance = 1 << 14;
	size_t final_index = 0;
	bool collision_found = true;

	while(min_distance > 1.0){
		if(position.x > SCREEN_WIDTH || position.x < 0 || position.y > SCREEN_HEIGHT || position.y < 0){
			collision_found = false;
			break;
		}

		min_distance = 1 << 14;
		for(size_t i = 0; i < NUMBER_OF_CIRCLES; i++){
			circle c = circles[i];
			double d = signed_distance_circle(position, c.position, c.radius);
			if(d < min_distance){
				min_distance = d;
				final_index = i;
			}
		}

		// Intermediate circles
		if(draw_debug_ray_circles){
			circleColor(renderer, position.x, position.y, abs(min_distance), DARK_GREEN);
			circleRGBA(renderer, position.x, position.y, abs(min_distance), HEX_TO_RGBA_COLOR(DARK_GREEN));
		}
		position = add(position, mul(abs(min_distance), direction));
	}

	vec2 circle_position = circles[final_index].position;
	vec2 collision_position = add(position, mul(abs(min_distance), direction));
	vec2 normal = circle_normal(circle_position, collision_position);
	//draw_vec2(collision_position, normal);
	return (raymarch_results){
		direction,
		start_position,
		collision_position, 
		circle_position, 
		normal, 
		circle_color(final_index), 
		collision_found,
		final_index};
}



void randomize_circles(circle* circles){
	for(size_t i = 0; i < NUMBER_OF_CIRCLES; i++){
		circle* c = &circles[i];
		c->position.x = rand() % SCREEN_WIDTH;
		c->position.y = rand() % SCREEN_HEIGHT;
		c->radius = 5 + rand() % 50;
	}
}

void move_circles(circle* circles){
	for(size_t i = 0; i < NUMBER_OF_CIRCLES; i++){
		circle* c = &circles[i];

		perlin_seed(i);
		double angle = 2*M_PI*perlin_noise(c->position.x/SCREEN_WIDTH, c->position.y/SCREEN_HEIGHT, (c->position.x+c->position.y+i)/(SCREEN_WIDTH + SCREEN_HEIGHT + NUMBER_OF_CIRCLES));
		vec2 d = {.x = cos(angle), .y = sin(angle)};

		c->position = add(c->position, mul(0.5, d));

		c->position.x = wrap(0, SCREEN_WIDTH, c->position.x);
		c->position.y = wrap(0, SCREEN_HEIGHT, c->position.y);
	}
}

int main(){
	srand(time(NULL));
	randomize_circles(circles);

	if(SDL_Init(SDL_INIT_VIDEO) != 0){
		fprintf(stderr, "Error SDL_Init(...): %s\n", SDL_GetError());
		exit(1);
	}

	SDL_Window* window = SDL_CreateWindow("Hello World!", 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if(window == NULL){
		fprintf(stderr, "Error SDL_CreateWindow(...): %s\n", SDL_GetError());
		SDL_Quit();
		exit(1);
	}

	renderer = SDL_CreateRenderer(window, -1 , SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if(renderer == NULL){
		fprintf(stderr, "Error SDL_CreateRenderer(...): %s\n", SDL_GetError());
		SDL_Quit();
		exit(1);
	}

	bool running = true;
	while(running){
		SDL_Event event;
		while(SDL_PollEvent(&event)){
			switch(event.type){
				case SDL_QUIT:
					running = false;
					break;
				case SDL_KEYDOWN:
					switch(event.key.keysym.sym){
						case SDLK_1:
							randomize_circles(circles);
							break;
						case SDLK_2:
							draw_debug_circles = !draw_debug_circles;
							break;
						case SDLK_3:
							draw_debug_rays = !draw_debug_rays;
							break;
						case SDLK_4:
							draw_debug_ray_circles = !draw_debug_ray_circles;
							break;
						case SDLK_5:
							light_pos = mouse;
							break;
						case SDLK_a:
							light_intensity++;
							break;
						case SDLK_s:
							if(light_intensity > 0)
								light_intensity--;
							break;
						case SDLK_q:
							if(raycount >= 10)
								raycount -= 10;
							break;
						case SDLK_w:
							if(raycount <= MAX_RAYCOUNT-10)
								raycount += 10;
							break;
					}
					break;
				case SDL_MOUSEMOTION:{
						int x = 0;
						int y = 0;
						SDL_GetMouseState(&x, &y);
						mouse.x = x;
						mouse.y = y;
						break;
				 }
			}
		}

		//move_circles(circles);

		SDL_SetRenderDrawColor(renderer, HEX_TO_RGBA_COLOR(BACKGROUND));
		SDL_RenderClear(renderer);

		position = mouse;
		if(draw_debug_circles){
			for(size_t i = 0; i < NUMBER_OF_CIRCLES; i++){
				circle c = circles[i];
				vec3 rgb = circle_color(i);
				//filledCircleColor(renderer, c.position.x, c.position.y, c.radius, 0xFF808080);
				filledCircleRGBA(renderer, c.position.x, c.position.y, c.radius, 255.0*rgb.x, 255.0*rgb.y, 255.0*rgb.z, 255);
			}
		}

		printf("raycount: %i, light_intensity: %i\r", raycount, light_intensity);

		double angle = 0.0;
		raymarch_results primary, normal_old, normal_new, light;
		for(int i = 0; i < raycount; i++){
			angle += 2*M_PI/raycount;
			primary = raymarch(position, make_vec2(cos(angle), sin(angle)));

			// debug
			if(draw_debug_rays){
				lineColor(renderer, primary.start_pos.x, primary.start_pos.y, primary.final_pos.x, primary.final_pos.y, DARK_GREEN);
			}

			normal_old = primary;
			normal_new = primary;

			// reflections
			for(int j = 0; normal_old.collision_found && j < 0; j++){
				vec2 d = normalize(reflect(normal_old.cast_direction, normal_old.object_normal));
				normal_new = raymarch(add(normal_old.final_pos, d), d);

				if(draw_debug_rays){
					lineColor(renderer, normal_new.start_pos.x, normal_new.start_pos.y, normal_new.final_pos.x, normal_new.final_pos.y, DARK_GREEN);
				}

				// color blending
				if(normal_new.collision_found){
					primary.object_color.x = (0.9*primary.object_color.x + 0.1*normal_new.object_color.x);
					primary.object_color.y = (0.9*primary.object_color.y + 0.1*normal_new.object_color.y);
					primary.object_color.z = (0.9*primary.object_color.z + 0.1*normal_new.object_color.z);
				}
				
				normal_old = normal_new;
			}

			// march to light
			//light = raymarch(primary.final_pos, normalize(sub(light_pos, primary.final_pos)));
			vec2 light_dir = normalize(sub(light_pos, primary.final_pos));
			//hitColor = hitObject->albedo / M_PI * light->intensity * light->color * std::max(0.f, hitNormal.dotProduct(L));
			primary.object_color = mul_vec3(light_intensity*0.25/M_PI * fmax(0.0, dot(primary.object_normal, light_dir)), primary.object_color);
			primary.object_color.x = fmin(1.0, primary.object_color.x);
			primary.object_color.y = fmin(1.0, primary.object_color.y);
			primary.object_color.z = fmin(1.0, primary.object_color.z);
			//primary.object_color = normalize_vec3(primary.object_color);

			raymarch_points[i] = primary;

			//filledCircleColor(renderer, raymarch_res[i].collision_position.x, raymarch_res[i].collision_position.y, 2, 0xFF808080);
		}

		// drawing object colors
		for(int i = 0; i < raycount; i++){
			vec2 p0 = raymarch_points[i].final_pos;
			vec3 c0 = raymarch_points[i].object_color;
			int id0 = raymarch_points[i].object_id;
			bool found0 = raymarch_points[i].collision_found;
			vec2 p1 = raymarch_points[(i+1<raycount)?i+1:0].final_pos;
			vec3 c1 = raymarch_points[(i+1<raycount)?i+1:0].object_color;
			int id1 = raymarch_points[(i+1<raycount)?i+1:0].object_id;
			bool found1 = raymarch_points[(i+1<raycount)?i+1:0].collision_found;
			//pixelRGBA(renderer, p0.x, p0.y, 255.0*c0.r, 255.0*c0.g, 255.0*c0.b, 255);
			//pixelRGBA(renderer, p1.x, p1.y, 255.0*c1.r, 255.0*c1.g, 255.0*c1.b, 255);
			// x0,y0 -> x1,y1
			// (y1-y0)/(x1-x0)
			if((found0 && found1) && (id0 == id1 || length(sub(p1,p0)) < 10.0)){
				vec2 dp = sub(p1,p0);
				vec3 dc = sub_vec3(c1,c0);

				for(double f = 0.0; f <= 1.0; f += 0.01){
					//pixelRGBA(renderer, p0.x+dx, p0.y+k*dx, 255.0*c0.r, 255.0*c0.g, 255.0*c0.b, 255);
					pixelRGBA(renderer, p0.x+f*dp.x, p0.y+f*dp.y, 
							255.0*(c0.x+f*dc.x), 
							255.0*(c0.y+f*dc.y),
							255.0*(c0.z+f*dc.z), 
							255);
				}
				//filledCircleColor(renderer, p0.x, p0.y, 2, 0xFFFF0000);
				//filledCircleColor(renderer, p1.x, p1.y, 2, 0xFF00FF00);
			}
			//filledCircleRGBA(renderer, p0.x, p0.y, 2, 255.0*c0);
		}

		filledCircleColor(renderer, light_pos.x, light_pos.y, 5, 0xFF00BBBB);
			
		//printf("%u, %u, %u, %u\n", HEX_TO_RGBA_COLOR(RED));
		//printf("%u\n", (Uint8) RED & 0xFF000000);
		//boxColor(renderer, 0,0,50,50, 0xFF0000FF);
		SDL_RenderPresent(renderer);
	}

	SDL_Quit();
}
