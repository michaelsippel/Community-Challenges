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

const float pi = 3.14159265358979f;
Player *p1;
Player *p2;

float rad2deg(float rad) {
    return (rad / pi) * 180.0f;
}

float deg2rad(float deg) {
    return (deg / 180.0f) * pi;
}

int main(int argc, char **argv) {
    // init
    oxygarum_set_resolution(1024, 576);
    oxygarum_set_title("Pong");
    init_oxygarum();

    // setup
    screen_t *screen = oxygarum_create_screen();
    scene_t *scene = oxygarum_create_scene();
    screen->width = screen->viewport.width = 1024;
    screen->height = screen->viewport.height = 576;
    screen->scene = scene;
    screen->camera->pos.y = -25.0;
    screen->camera->fov = 90;

    // scene setup
    struct load_return *ret = oxygarum_load_oxy3d_file("data/scene.oxy3d", NULL);

    object3d_t *arena = oxygarum_create_object3d();
    arena->mesh = (mesh3d_t*) oxygarum_get_group_entry(ret->meshes, "arena")->element;
    arena->pos.y = -1;
    
    Ball *ball = new Ball();
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
    
    SDL_ShowCursor(0);
    float a1 = 0, a2 = 0;

    glEnable(GL_CULL_FACE);
    
    // main loop
    while (1) {
        // update (calculate frametime, handle events, etc.)
        float frametime = oxygarum_update();

        ball->update(frametime);
        p1->update(frametime);
        p2->update(frametime);

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

        if(buttons & SDL_BUTTON(1)) {
            controler_click_left();
        }
        if(buttons & SDL_BUTTON(3)) {
            controler_click_right();
        }
        
        // render
        oxygarum_render_screen(screen);
    }

    return 0;
}
