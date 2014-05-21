#include <stdio.h>
#include <oxygarum.h>

#include "chunk.h"
#include "materials.h"
#include "player.h"

using namespace oxygarum;

Player *player;
int a_pressed = 0;
int s_pressed = 0;
int d_pressed = 0;

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
		1.0f / (float)gx - 1.0f/64.0f,
		1.0f / (float)gy - 1.0f/64.0f
	);

	texcoords[3] = Vector2D(loc.x,loc.y - size.y);
	texcoords[2] = Vector2D(loc.x+ size.x, loc.y - size.y);
	texcoords[1] = Vector2D(loc.x+ size.x, loc.y);
	texcoords[0] = Vector2D(loc.x, loc.y);
}

void keyboard_down(SDL_Event *e)
{
	switch(e->key.keysym.sym)
	{
		case 'a':
			a_pressed = 1;
			break;
		case 'd':
			d_pressed = 1;
			break;
		case ' ':
		case 's':
			s_pressed = 1;
			break;
	}
}

void keyboard_up(SDL_Event *e)
{
	switch(e->key.keysym.sym)
	{
		case 'a':
			a_pressed = 0;
			break;
		case 'd':
			d_pressed = 0;
			break;
		case ' ':
		case 's':
			s_pressed = 0;
			break;
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
	camera->position = Vector3D(0.0f, -10.0f, -25.0f);

	Material *mat = new Material();
	Texture *tex = loader::load_texture("data/texture.png");
	tex->params->add(new TextureParameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST));
	tex->params->add(new TextureParameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST));
	tex->load();
	mat->map_texture(tex, "", 0);
	
	glEnable(GL_DEPTH_TEST);

	Chunk *chunks[32];
	Chunk *hills1[32];
	Chunk *hills2[32];
	Chunk *clouds[32];

	int i;
	for(i =0; i<32; i++)
	{
		chunks[i] = new Chunk();
		chunks[i]->id = i;
		hills1[i] = new Chunk();
		hills2[i] = new Chunk();
		clouds[i] = new Chunk();

		// generate playable terrain
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
					if(y == height+1)
					{
						int candy = rand()%16;
						switch(candy)
						{
							case 0:
								chunks[i]->blocks[x][y] = CHOCO;
								break;
							case 1:
								chunks[i]->blocks[x][y] = BONBON;
								break;
							case 2:
								chunks[i]->blocks[x][y] = GUMMI;
								break;
						}
					}

					if( (i == 0 && x == 0) ||
					    (i ==31 && x ==15) )
					{
						chunks[i]->blocks[x][y] = STONE;
					}
				}
			}
		}

		// generate Hills
		for(x = 0; x < CHUNK_SIZE_X; x++)
		{
			int nx1 = x + (i+12234)*CHUNK_SIZE_X;
			int nx2 = x + (i+23453)*CHUNK_SIZE_X;
			float noise1 = perlin_noise(nx1);
			float noise2 = perlin_noise(nx2);
			int height1 = (int)1+ ( noise1 * (float)CHUNK_SIZE_Y-2.0f )*1.5f;
			int height2 = (int)2+ ( noise2 * (float)CHUNK_SIZE_Y-2.0f )*1.5f;

			for(y = 0; y < CHUNK_SIZE_Y; y++)
			{
				int c = rand()%4;
				switch(c)
				{
					case 0:
						clouds[i]->blocks[x][y] = SKY1;
						break;
					case 1:
						clouds[i]->blocks[x][y] = SKY2;
						break;
					case 2:
						clouds[i]->blocks[x][y] = SKY3;
						break;
					case 3:
						clouds[i]->blocks[x][y] = SKY4;
						break;
				}

				if(y == height1)
				{
					hills1[i]->blocks[x][y] = GRASS;
				}
				else if(y == height1-1)
				{
					hills1[i]->blocks[x][y] = DIRT;
				}
				else if(y < height1)
				{
					hills1[i]->blocks[x][y] = STONE;
				}
				else
				{
					hills1[i]->blocks[x][y] = NONE;
				}

				if(y == height2)
				{
					hills2[i]->blocks[x][y] = GRASS;
				}
				else if(y == height2-1)
				{
					hills2[i]->blocks[x][y] = DIRT;
				}
				else if(y < height2)
				{
					hills2[i]->blocks[x][y] = STONE;
				}
				else
				{
					hills2[i]->blocks[x][y] = NONE;
				}

			}
		}

		hills1[i]->generate_mesh();
		hills1[i]->obj->material = mat;

		hills1[i]->obj->scaling = Vector3D(1.5f, 1.5f, 1.0f);
		hills1[i]->obj->position.x = -10.0f + 16*i;
		hills1[i]->obj->position.y = -4.0f;
		hills1[i]->obj->position.z = -10.0f;

		hills2[i]->generate_mesh();
		hills2[i]->obj->material = mat;

		hills2[i]->obj->scaling = Vector3D(2.0f, 2.0f, 1.0f);
		hills2[i]->obj->position.x = -10.0f + 16*i;
		hills2[i]->obj->position.y = -2.0f;
		hills2[i]->obj->position.z = -20.0f;

		clouds[i]->generate_mesh();
		clouds[i]->obj->material = mat;
		clouds[i]->obj->scaling = Vector3D(4.0f, 4.0f, 1.0f);
		clouds[i]->obj->position.x = -10.0f + 16*i;
		clouds[i]->obj->position.y = -4.0f;
		clouds[i]->obj->position.z = -40.0f;
		chunks[i]->generate_mesh();
		chunks[i]->obj->material = mat;
		chunks[i]->obj->setFlag(OBJECT_TRANSPARENT);
		
		scene->objects3D->add(hills1[i]->obj);
		scene->objects3D->add(hills2[i]->obj);
		scene->objects3D->add(clouds[i]->obj);

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

		if(a_pressed) player->move(-1);
		else if(d_pressed) player->move(1);
		else player->move(0);
		if(s_pressed) player->jump(2);

		player->update(frametime*0.005, chunks);
		camera->position.x = - player->obj->position.x;
	}

	return 0;
}

