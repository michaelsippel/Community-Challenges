#include <stdio.h>
#include <oxygarum.h>

#include "chunk.h"
#include "materials.h"

using namespace oxygarum;

void quit(SDL_Event *e)
{
	printf("Exiting..\n");
	exit(0);
}

int main(void)
{
	printf("Hello Candy-World!\n");

	SDLWindow *window = new SDLWindow("CandyCourier", 800,600);
	EventManager *eventmanager = new EventManager();

	eventmanager->register_handler(SDL_QUIT, quit);

	Scene *scene = new Scene();
	Camera *camera = new Camera(window, scene);
	camera->position = Vector3D(0.0f, -5.0f, -15.0f);

	Chunk *testchunk = new Chunk();

	int x,y;
	for(x = 0; x < CHUNK_SIZE_X; x++)
	{
		for(y = 0; y < CHUNK_SIZE_Y; y++)
		{
			if(y == 0)
			{
				testchunk->blocks[x][y] = STONE;
			}
			else if(y < 4)
			{
				testchunk->blocks[x][y] = DIRT;
			}
			else if(y < 5 && x%2 == 0)
			{
				testchunk->blocks[x][y] = GRASS;
			}
			else
			{
				testchunk->blocks[x][y] = NONE;
			}
		}
	}

	testchunk->generate_mesh();
	scene->objects3D->add(testchunk->obj);

	while(1)
	{
		// handle events
		eventmanager->poll_events();

		// render
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		camera->render();

		// update
		float frametime = window->update();
	}

	return 0;
}

