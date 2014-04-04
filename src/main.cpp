/**
 *  main.c
 *
 *  (C) Copyright 2014 Michael Sippel
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <oxygarum.h>
#include <math.h>

#include "Ball.h"
#include "Player.h"
#include "Controler.h"
#include "AI.h"

const float pi = 3.14159265358979f;
Player *p1;
Player *p2;
Ball *ball;

char s_energy1[64];
char s_energy2[64];
char s_score[16];
char s_middle[256];

float rad2deg(float rad) {
    return (rad / pi) * 180.0f;
}

float deg2rad(float deg) {
    return (deg / 180.0f) * pi;
}

int main(int argc, char **argv) {
    float bot_strength = 0;
    printf("Please enter Bot strength (0 - 100, 50 is optimum): ");
    scanf("%f", &bot_strength);

    // init
    oxygarum_set_resolution(1280, 720);
    oxygarum_set_title("EnergyDefense - Pong");
    init_oxygarum();

    // setup
    screen_t *screen = oxygarum_create_screen();
    scene_t *scene = oxygarum_create_scene();
    screen->width = screen->viewport.width = 1366;
    screen->height = screen->viewport.height = 768;
    screen->scene = scene;
    screen->camera->pos.y = -25.0;
    screen->camera->fov = 90;

    // scene setup
    struct load_return *ret = oxygarum_load_oxy3d_file("data/scene.oxy3d", NULL);

    object3d_t *arena = oxygarum_create_object3d();
    arena->mesh = (mesh3d_t*) oxygarum_get_group_entry(ret->meshes, "arena")->element;
    arena->pos.y = -1;

    ball = new Ball();
    ball->object->mesh = (mesh3d_t*) oxygarum_get_group_entry(ret->meshes, "ball")->element;

    p1 = new Player();
    p1->racket->mesh = (mesh3d_t*) oxygarum_get_group_entry(ret->meshes, "racket")->element;
    p1->racket->material = (material_t*) oxygarum_get_group_entry(ret->materials, "material_racket_red")->element;
    p1->racket->pos.x = -20;
    p1->racket->rot.y = 90;

    p1->plasma1->mesh = (mesh3d_t*) oxygarum_get_group_entry(ret->meshes, "plasma")->element;
    p1->plasma1->material = (material_t*) oxygarum_get_group_entry(ret->materials, "material_plasma_red")->element;
    p1->plasma1->status |= OBJECT_TRANSPARENT;
    p1->plasma1->pos.x = -25;
    p1->plasma1->pos.z = -18;

    p1->plasma2->mesh = (mesh3d_t*) oxygarum_get_group_entry(ret->meshes, "plasma")->element;
    p1->plasma2->material = (material_t*) oxygarum_get_group_entry(ret->materials, "material_plasma_red")->element;
    p1->plasma2->status |= OBJECT_TRANSPARENT;
    p1->plasma2->pos.x = -25;
    p1->plasma2->pos.z = 20;


    p2 = new Player();
    p2->racket->mesh = (mesh3d_t*) oxygarum_get_group_entry(ret->meshes, "racket")->element;
    p2->racket->material = (material_t*) oxygarum_get_group_entry(ret->materials, "material_racket_blue")->element;
    p2->racket->pos.x = 20;
    p2->racket->rot.y = 90;

    p2->plasma1->mesh = (mesh3d_t*) oxygarum_get_group_entry(ret->meshes, "plasma")->element;
    p2->plasma1->material = (material_t*) oxygarum_get_group_entry(ret->materials, "material_plasma_blue")->element;
    p2->plasma1->status |= OBJECT_TRANSPARENT;
    p2->plasma1->pos.x = 25;
    p2->plasma1->pos.z = -20;

    p2->plasma2->mesh = (mesh3d_t*) oxygarum_get_group_entry(ret->meshes, "plasma")->element;
    p2->plasma2->material = (material_t*) oxygarum_get_group_entry(ret->materials, "material_plasma_blue")->element;
    p2->plasma2->status |= OBJECT_TRANSPARENT;
    p2->plasma2->pos.x = 25;
    p2->plasma2->pos.z = 20;

    oxygarum_group_add(scene->objects3d, arena, NULL);
    oxygarum_group_add(scene->objects3d, ball->object, NULL);
    oxygarum_group_add(scene->objects3d, p1->racket, NULL);
    oxygarum_group_add(scene->objects3d, p1->plasma1, NULL);
    oxygarum_group_add(scene->objects3d, p1->plasma2, NULL);
    oxygarum_group_add(scene->objects3d, p2->racket, NULL);
    oxygarum_group_add(scene->objects3d, p2->plasma1, NULL);
    oxygarum_group_add(scene->objects3d, p2->plasma2, NULL);

    oxygarum_set_keyboard_event('a', &controler_left);
    oxygarum_set_keyboard_event('d', &controler_right);
    oxygarum_set_keyboard_event_up('a', &controler_up);
    oxygarum_set_keyboard_event_up('d', &controler_up);

    texture_t *font_tex = oxygarum_load_texture_from_file("data/font.png", NULL);
    font_t *font = oxygarum_create_font(font_tex, 11, 11, ' ', 14);
    text_t *t_energy1 = oxygarum_create_text(s_energy1, font, 0, screen->height - 50);
    text_t *t_energy2 = oxygarum_create_text(s_energy2, font, screen->width - 400, screen->height - 50);
    text_t *t_score = oxygarum_create_text(s_score, font, screen->width / 2 - 60, screen->height - 50);
    t_energy1->color.rgb.g = 0;
    t_energy1->color.rgb.b = 0;
    t_energy2->color.rgb.r = 0;
    t_energy2->color.rgb.g = 0;
    t_score->color.rgb.r = 0;
    t_score->color.rgb.b = 0;
    oxygarum_group_add(scene->texts, t_energy1, NULL);
    oxygarum_group_add(scene->texts, t_energy2, NULL);
    oxygarum_group_add(scene->texts, t_score, NULL);

    SDL_ShowCursor(0);
    float a1 = 0, a2 = 0;

    glEnable(GL_CULL_FACE);

    // create bot
    AI *bot = new AI(p2, bot_strength);
    ball->reset(p1);

    // main loop
    while (1) {
        // update (calculate frametime, handle events, etc.)
        float frametime = oxygarum_update();

        bot->update(ball->get_pos(), frametime);

        ball->update(frametime);
        p1->update(frametime);
        p2->update(frametime);

        int power1 = (int) p1->get_power();
        int power2 = (int) p2->get_power();
        sprintf(s_energy1, "%d %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c|", power1,
                power1 / 5 ? '=' : ' ', power1 / 10 ? '=' : ' ',
                power1 / 15 ? '=' : ' ', power1 / 20 ? '=' : ' ',
                power1 / 25 ? '=' : ' ', power1 / 30 ? '=' : ' ',
                power1 / 35 ? '=' : ' ', power1 / 40 ? '=' : ' ',
                power1 / 45 ? '=' : ' ', power1 / 50 ? '=' : ' ',
                power1 / 55 ? '=' : ' ', power1 / 60 ? '=' : ' ',
                power1 / 65 ? '=' : ' ', power1 / 70 ? '=' : ' ',
                power1 / 75 ? '=' : ' ', power1 / 80 ? '=' : ' ',
                power1 / 85 ? '=' : ' ', power1 / 90 ? '=' : ' ',
                power1 / 95 ? '=' : ' ', power1 / 100 ? '=' : ' ');
        sprintf(s_energy2, "|%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c %d",
                power2 / 100 ? '=' : ' ', power2 / 95 ? '=' : ' ',
                power2 / 90 ? '=' : ' ', power2 / 85 ? '=' : ' ',
                power2 / 80 ? '=' : ' ', power2 / 75 ? '=' : ' ',
                power2 / 70 ? '=' : ' ', power2 / 65 ? '=' : ' ',
                power2 / 60 ? '=' : ' ', power2 / 55 ? '=' : ' ',
                power2 / 50 ? '=' : ' ', power2 / 45 ? '=' : ' ',
                power2 / 40 ? '=' : ' ', power2 / 35 ? '=' : ' ',
                power2 / 30 ? '=' : ' ', power2 / 25 ? '=' : ' ',
                power2 / 20 ? '=' : ' ', power2 / 15 ? '=' : ' ',
                power2 / 10 ? '=' : ' ', power2 / 5 ? '=' : ' ',
                power2);

        if (power1 <= 0.0f) {
            p2->score_up();

            p1->reset();
            p2->reset();
            ball->reset(p1);
        }

        if (power2 <= 0.0f) {
            p1->score_up();

            p1->reset();
            p2->reset();
            ball->reset(p2);
        }

        sprintf(s_score, "%d:%d", p1->get_score(), p2->get_score());

        // mouse control
        int mx, my;
        int buttons = SDL_GetMouseState(&mx, &my);
        float dist = 40;

        a1 = 180 + ((float) mx / screen->width) * 360.0f; //  * frametime;
        a2 = 0 - ((float) my / screen->height) * 90.0f; //  * frametime;
        screen->camera->rot.x = 90 + a2;
        screen->camera->rot.y = a1;

        screen->camera->pos.x = sin(deg2rad(a1)) * dist;
        screen->camera->pos.y = -cos(deg2rad(a2)) * dist;
        screen->camera->pos.z = -cos(deg2rad(a1)) * dist;

        if (buttons & SDL_BUTTON(1)) {
            controler_click_left();
        }
        if (buttons & SDL_BUTTON(3)) {
            controler_click_right();
        }

        // render
        oxygarum_render_screen(screen);
    }

    return 0;
}
