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

	while(1)
	{
        // handle events
		eventmanager->poll_events();

        // render
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER);
        camera->render();

        // update
		float frametime = window->update();
	}

	return 0;
}

