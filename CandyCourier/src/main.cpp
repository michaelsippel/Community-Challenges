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

float noise(int x)
{
	x = (x<<13) ^ x;
    float n = (float) ( 1.0 - ( (x * (x * x * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0);

	return 0.5f + n/2.0f;
}

float lerp(float a,float b,float x)
{
	return a * ( 1 - x ) + b * x;
}

float interpolated_noise(int x, int freq)
{
    int x1 = x - (x%freq);
	int x2 = x1 + freq;
	float diff = (float)(x%freq)/(float)freq;

    float n1 = noise(x1);
    float n2 = noise(x2);
    float n = lerp(n1, n2, diff);
	
    return n;
}

float perlin_noise(int x)
{
	int i;
    float n = 0;

    float amp = 1.0f;
	float persistence = 0.5;
	int octaves = 2;
    unsigned int freq = 16;

    for(i = 0; i < octaves; i++) {
        n += interpolated_noise(x, freq) * amp;
        freq /= 2;
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
			int nx = x + i*CHUNK_SIZE_X;
			float noise = perlin_noise(nx);
			int height = (int) ( noise * (float)CHUNK_SIZE_Y-2.0f );
			//printf("height: %d, noise: %f\n", height, noise);
			for(y = 0; y < CHUNK_SIZE_Y; y++)
			{
				if(y == height)
				{
					chunks[i]->blocks[x][y] = GRASS;
				}
				else if(y == height-1)
				{
					chunks[i]->blocks[x][y] = DIRT;
				}
				else if(y < height)
				{
					chunks[i]->blocks[x][y] = STONE;
				}
				else
				{
					chunks[i]->blocks[x][y] = NONE;
					if( (i == 0 && x == 0) ||
					    (i ==31 && x ==15) )
					{
						chunks[i]->blocks[x][y] = STONE;
					}
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

