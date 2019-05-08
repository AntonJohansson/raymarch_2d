#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
/* Ensure we are using opengl's core profile only */
#define GL3_PROTOTYPES 1
#include <glad/glad.h>
#include "shader.h"
#include "gl_error_checking.h"
#include "vec2.h"

#include <SDL.h>

#define SCREEN_WIDTH 512
#define SCREEN_HEIGHT 512

#define MAX_RAYCOUNT 10000
#define RAYCOUNT_STEPSIZE 10
static int raycount = 0;

static vec2 mouse = {SCREEN_WIDTH/2, SCREEN_HEIGHT/2};

void sdldie(char *msg)
{
    printf("%s: %s\n", msg, SDL_GetError());
    SDL_Quit();
    exit(1);
}

void setupwindow(SDL_Window **window, SDL_GLContext *context){
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
        sdldie("Unable to initialize SDL");

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    *window = SDL_CreateWindow("test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    if (!*window)
        sdldie("Unable to create window");

    *context = SDL_GL_CreateContext(*window);

    // This makes our buffer swap syncronized with the monitor's vertical refresh */
    SDL_GL_SetSwapInterval(1);

		// Initialize GLAD after the window context has been created.
		if (!gladLoadGLLoader((GLADloadproc) SDL_GL_GetProcAddress)){
			sdldie("glad fucked up");
		}
}

void drawscene(SDL_Window* window){
    GLuint vao, vbo, ebo; /* Create handles for our Vertex Array Object and two Vertex Buffer Objects */

    const GLfloat vbo_data[] = {
			-1.0, -1.0, 	0.0, 0.0,
			 1.0, -1.0,		1.0, 0.0,
			 1.0,  1.0, 	1.0, 1.0,
			-1.0,  1.0,		0.0, 1.0
		};

		const GLuint ebo_data[] = {
			0, 1, 2,
			0, 2, 3
		};

		// VAO, VBO, EBO
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vbo_data), &vbo_data[0], GL_STATIC_DRAW);

		glGenBuffers(1, &ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ebo_data), &ebo_data[0], GL_STATIC_DRAW);

		// Texture
		GLuint texture;
		glGenTextures(1, &texture);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
		glBindImageTexture(0, texture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);

		// Work group stuff
		GLuint work_group_count[3], work_group_size[3], work_group_invocations;
		glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 0, &work_group_count[0]);
		glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 1, &work_group_count[1]);
		glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 2, &work_group_count[2]);
		glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, &work_group_size[0]);
		glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 1, &work_group_size[1]);
		glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 2, &work_group_size[2]);
		glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, &work_group_invocations);

		printf("max global (total) work group size: x: %i, y: %i, z: %i\n",
				work_group_count[0],
				work_group_count[1],
				work_group_count[2]
				);
		printf("max local (in one shader) work group sizes x: %i, y: %i, z: %i\n",
				work_group_size[0],
				work_group_size[1],
				work_group_size[2]
				);
		printf("max local work group invocations %i\n", work_group_invocations);

		// Compute program
		GLuint compute = program_begin();
		program_attach_shader_from_file("../res/compute.comp", GL_COMPUTE_SHADER);
		program_link();
		program_end();

		GLuint clear = program_begin();
		program_attach_shader_from_file("../res/compute_clear_image.comp", GL_COMPUTE_SHADER);
		program_link();
		program_end();


		// Shader program
		GLuint program = program_begin();
		program_attach_shader_from_file("../res/tutorial2.vert", GL_VERTEX_SHADER);
		program_attach_shader_from_file("../res/tutorial2.frag", GL_FRAGMENT_SHADER);
		program_bind_attribute(0, 2, GL_FLOAT, GL_FALSE, 4*sizeof(GL_FLOAT), 0);
		program_bind_attribute(1, 2, GL_FLOAT, GL_FALSE, 4*sizeof(GL_FLOAT), 2*sizeof(GL_FLOAT));
		program_link();
		program_end();

    glUseProgram(program);

		int counted_frames = 0;
		int start = SDL_GetTicks();

		bool running = true;
		while(running){
			counted_frames++;
			float avg_fps = counted_frames/((SDL_GetTicks() - start)/1000.0f);
			printf("fps: %f, raycount: %i\r",avg_fps, raycount);

			SDL_Event event;
			while(SDL_PollEvent(&event)){
				switch(event.type){
					case SDL_QUIT:
						running = false;
						break;
					case SDL_KEYDOWN:
						switch(event.key.keysym.sym){
							case SDLK_q:
								running = false;
								break;
							//case SDLK_1:
							//	randomize_circles(circles);
							//	break;
							//case SDLK_2:
							//	draw_debug_circles = !draw_debug_circles;
							//	break;
							//case SDLK_3:
							//	draw_debug_rays = !draw_debug_rays;
							//	break;
							//case SDLK_4:
							//	draw_debug_ray_circles = !draw_debug_ray_circles;
							//	break;
							//case SDLK_5:
							//	light_pos = mouse;
							//	break;
							//case SDLK_a:
							//	light_intensity++;
							//	break;
							//case SDLK_s:
							//	if(light_intensity > 0)
							//		light_intensity--;
							//	break;
							case SDLK_DOWN:
								if(raycount > RAYCOUNT_STEPSIZE)
									raycount -= RAYCOUNT_STEPSIZE;
								break;
							case SDLK_UP:
								if(raycount < MAX_RAYCOUNT-RAYCOUNT_STEPSIZE)
									raycount += RAYCOUNT_STEPSIZE;
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
			// Compute
			glUseProgram(clear);
			glDispatchCompute(512, 512, 1);
			glUseProgram(compute);
			glUniform2f(0, mouse.x, SCREEN_HEIGHT - mouse.y);
			glDispatchCompute(raycount, 1, 1);
			//glDispatchCompute(SCREEN_WIDTH, SCREEN_HEIGHT, 1);

			glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

			// Drawing
			glClearColor(0.0, 0.0, 0.0, 1.0);
			glClear(GL_COLOR_BUFFER_BIT);

			glUseProgram(program);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
				glDrawElements(GL_TRIANGLES, sizeof(ebo_data)/sizeof(GLuint), GL_UNSIGNED_INT, NULL);

        SDL_GL_SwapWindow(window);
    }

    glUseProgram(0);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDeleteProgram(program);
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
}

void destroywindow(SDL_Window* window, SDL_GLContext* context){
    SDL_GL_DeleteContext(*context);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

/* Our program's entry point */
int main(int argc, char *argv[])
{
    SDL_Window* mainwindow; /* Our window handle */
    SDL_GLContext maincontext; /* Our opengl context handle */

    /* Create our window, opengl context, etc... */
    setupwindow(&mainwindow, &maincontext);

		setup_gl_error_callback();
    /* Call our function that performs opengl operations */
    drawscene(mainwindow);

    /* Delete our opengl context, destroy our window, and shutdown SDL */
    destroywindow(mainwindow, &maincontext);

    return 0;
}
