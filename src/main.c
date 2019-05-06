#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include "perlin.h"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

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

#define HEX_TO_RGBA_COLOR(hex) \
	(hex >> 0) & 0xFF,\
	(hex >> 8) & 0xFF,\
	(hex >> 16) & 0xFF,\
	(hex >> 24) & 0xFF 

typedef struct vec2{
	double x, y;
} vec2;

typedef struct circle{
	vec2 position;
	Sint16 radius;
} circle;

#define SIZE 16
static circle circles[SIZE];

double angle = 0.0f;

static vec2 position = {
	.x = SCREEN_WIDTH/2,
	.y = SCREEN_HEIGHT/2,
};
static vec2 mouse = {
	.x = 1.1*SCREEN_WIDTH/2,
	.y = 1.0*SCREEN_HEIGHT/2,
};

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

double bound(double lower, double upper, double value){
	return fmin(fmax(lower, value), upper);
}

vec2 normalize(vec2 a){
	double l = length(a);
	vec2 v = {.x = a.x/l, .y = a.y/l};
	return v;
}






double signed_distance_circle(vec2 p0, vec2 p1, Sint16 r){
	return length(sub(p1,p0))-r;
}








void randomize_circles(circle* circles){
	for(size_t i = 0; i < SIZE; i++){
		circle* c = &circles[i];
		c->position.x = rand() % SCREEN_WIDTH;
		c->position.y = rand() % SCREEN_HEIGHT;
		c->radius = 5 + rand() % 50;
	}
}

void move_circles(circle* circles){
	for(size_t i = 0; i < SIZE; i++){
		circle* c = &circles[i];

		double angle = 2*M_PI*perlin_noise(c->position.x/SCREEN_WIDTH, c->position.y/SCREEN_HEIGHT, (c->position.x+c->position.y+i)/(SCREEN_WIDTH + SCREEN_HEIGHT + SIZE));
		vec2 d = {.x = cos(angle), .y = sin(angle)};
		c->position = add(c->position, mul(1.0, d));
		c->position.x = bound(0, SCREEN_WIDTH, c->position.x);
		c->position.y = bound(0, SCREEN_HEIGHT, c->position.y);
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

	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1 , SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
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
					randomize_circles(circles);
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

		move_circles(circles);

		SDL_SetRenderDrawColor(renderer, HEX_TO_RGBA_COLOR(PEACH));
		SDL_RenderClear(renderer);

		for(size_t i = 0; i < SIZE; i++){
			circle c = circles[i];
			filledCircleColor(renderer, c.position.x, c.position.y, c.radius, RED);
			filledCircleColor(renderer, c.position.x, c.position.y, c.radius-2, PEACH);
		}

		vec2 n = normalize(sub(mouse, position));
		vec2 begin_pos = position;
		double min_distance = 1 << 14;

		while(min_distance > 1.0 && begin_pos.x <= SCREEN_WIDTH && begin_pos.x >= 0 && begin_pos.y <= SCREEN_HEIGHT && begin_pos.y >= 0){
			min_distance = 1 << 14;
			for(size_t i = 0; i < SIZE; i++){
				circle c = circles[i];
				double d = signed_distance_circle(begin_pos, c.position, c.radius);
				if(d < min_distance){
					min_distance = d;
				}
			}

			//circleColor(renderer, begin_pos.x, begin_pos.y, abs(min_distance), DARK_GREEN);
			//circleRGBA(renderer, begin_pos.x, begin_pos.y, abs(min_distance), HEX_TO_RGBA_COLOR(DARK_GREEN));
			begin_pos = add(begin_pos, mul(abs(min_distance), n));
		}

		vec2 v = mul(abs(min_distance), n);

		lineColor(renderer, position.x, position.y, begin_pos.x, begin_pos.y, DARK_GREEN);
		filledCircleColor(renderer, begin_pos.x, begin_pos.y, 10, GREEN);
		
		//printf("%u, %u, %u, %u\n", HEX_TO_RGBA_COLOR(RED));
		//printf("%u\n", (Uint8) RED & 0xFF000000);
		//boxColor(renderer, 0,0,50,50, 0xFF0000FF);
		SDL_RenderPresent(renderer);
	}

	SDL_Quit();
}
