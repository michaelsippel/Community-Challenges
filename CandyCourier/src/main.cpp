#include <stdio.h>
#include <oxygarum.h>

#include "chunk.h"
#include "materials.h"
#include "player.h"

using namespace oxygarum;

Player *player;

void quit(SDL_Event *e)
{
	printf("Exiting..\n");
	exit(0);
}

void gen_texcoords(Vector2D *texcoords, int x, int y, int gx, int gy)
{
	Vector2D loc = Vector2D(
		(float)x / (float)gx,
		1.0f - ((float)y / (float)gy)
	);
	Vector2D size = Vector2D(
		1.0f / (float)gx,
		1.0f / (float)gy
	);

	texcoords[3] = Vector2D(loc.x +1.0f/64.0f,loc.y - size.y);
	texcoords[2] = Vector2D(loc.x + size.x, loc.y - size.y);
	texcoords[1] = Vector2D(loc.x + size.x, loc.y);
	texcoords[0] = Vector2D(loc.x +1.0f/64.0f, loc.y);
}

void keyboard_down(SDL_Event *e)
{
	switch(e->key.keysym.sym)
	{
		case 'a':
			player->move(-1);
			break;
		case 'd':
			player->move(1);
			break;
		case ' ':
		case 's':
			player->jump(2);
			break;
	}
}

void keyboard_up(SDL_Event *e)
{
	if(e->key.keysym.sym == 'a' ||
	   e->key.keysym.sym == 'd')
	{
		player->move(0);
	}
}

float noise2(int x, int y)
{
	int n=(int)x+(int)y*57;
	n=(n<<13)^n;
	int nn=(n*(n*n*60493+19990303)+1376312589)&0x7fffffff;
	return 1.0-((float)nn/1073741824.0);
}

inline float interpolate(float a,float b,float x)
{
	float ft=x * 3.1415927;
	float f=(1.0-cos(ft))* 0.5;
	return a*(1.0-f)+b*f;
}


float smooth_noise(float x,float y)
{
	float floorx=(float)((int)x);
	float floory=(float)((int)y);
	float e, s,t,u,v;

	s = noise2(floorx,floory); 
	t = noise2(floorx+1,floory);
	u = noise2(floorx,floory+1);
	v = noise2(floorx+1,floory+1);

	float int1=interpolate(s,t,x-floorx);
	float int2=interpolate(u,v,x-floorx);
	return interpolate(int1,int2,y-floory);
}

float perlin_noise(float x, float y)
{
	int i;
    float n = 0;

    float amp = 1.0f;
	float persistence = 0.5;
	int octaves = 4;
    unsigned int freq = 2;

    for(i = 0; i < octaves; i++) {
        n += smooth_noise(x*freq, y*freq) * amp;
        freq *= 2;
        amp *= persistence;
    }

    return (float) (n / (float)octaves);
}

int main(void)
{
	printf("Hello Candy-World!\n");

	SDLWindow *window = new SDLWindow("CandyCourier", 800,600);
	EventManager *eventmanager = new EventManager();

	eventmanager->register_handler(SDL_QUIT, quit);
	eventmanager->register_handler(SDL_KEYDOWN, keyboard_down);
	eventmanager->register_handler(SDL_KEYUP, keyboard_up);

	player = new Player();

	Scene *scene = new Scene();
	Camera *camera = new Camera(window, scene);
	camera->position = Vector3D(0.0f, -8.0f, -25.0f);


	Material *mat = new Material();
	Texture *tex = loader::load_texture("data/texture.png");
	tex->params->add(new TextureParameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST));
	tex->params->add(new TextureParameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST));
	tex->load();
	mat->map_texture(tex, "", 0);

	Chunk *chunks[32];

	int i;
	for(i =0; i<32; i++)
	{
		chunks[i] = new Chunk();

		int x,y;
		for(x = 0; x < CHUNK_SIZE_X; x++)
		{
			for(y = 0; y < CHUNK_SIZE_Y; y++)
			{
				float fx = (float)x / (float)CHUNK_SIZE_X;
				float fy = (float)y / (float)CHUNK_SIZE_Y;

				float dx = (fx - 0.5f) * 2.0f;
				float dy = (fy - 0.5f) * 2.0f;
				float n = 1-perlin_noise(fx,fy);
				float val = dy + n;

				if(val < 0.7f)
				{
					chunks[i]->blocks[x][y] = STONE;
				}
				else
				{
					chunks[i]->blocks[x][y] = NONE;
				}
			}
		}

		chunks[i]->generate_mesh();
		chunks[i]->obj->material = mat;
	
		scene->objects3D->add(chunks[i]->obj);
	}
	
	player->obj->material = mat;
	scene->objects3D->add(player->obj);

	while(1)
	{
		// handle events
		eventmanager->poll_events();

		// render
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		camera->render();

		// update
		float frametime = window->update();
		player->update(frametime*0.005, chunks);
		camera->position.x = - player->obj->position.x;
	}

	return 0;
}

