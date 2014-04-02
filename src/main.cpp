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

#include "Ball.h"
#include "Player.h"

int main(int argc, char **argv) {
    // init
    oxygarum_set_resolution(1024, 576);
    oxygarum_set_title("Pong");
    init_oxygarum();

    // setup
    screen_t *screen = oxygarum_create_screen();
    scene_t *scene = oxygarum_create_scene();
    screen->width = screen->viewport.width = 1024;
    screen->height =screen->viewport.height= 576;
    screen->scene = scene;
    screen->camera->pos.y = -25.0;
    screen->camera->rot.x = 90;
    screen->camera->fov = 90;
    
    // scene setup
    struct load_return *ret = oxygarum_load_oxy3d_file("data/scene.oxy3d", NULL);
    
    Ball *ball = new Ball();
    ball->object->mesh = (mesh3d_t*)oxygarum_get_group_entry(ret->meshes, "ball")->element;
    
    Player *p1 = new Player();
    p1->racket->mesh = (mesh3d_t*) oxygarum_get_group_entry(ret->meshes, "racket")->element;
    p1->racket->material = (material_t*) oxygarum_get_group_entry(ret->materials, "material_racket_red")->element;
    p1->racket->pos.x = -20;
    p1->racket->rot.y = 90;
    
    p1->plasma1->mesh = (mesh3d_t*) oxygarum_get_group_entry(ret->meshes, "plasma")->element;
    p1->plasma1->material = (material_t*) oxygarum_get_group_entry(ret->materials, "material_plasma_red")->element;
    p1->plasma1->status |= OBJECT_TRANSPARENT;
    p1->plasma1->pos.x = -21;
    p1->plasma1->pos.z = -10;
    
    p1->plasma2->mesh = (mesh3d_t*) oxygarum_get_group_entry(ret->meshes, "plasma")->element;
    p1->plasma2->material = (material_t*) oxygarum_get_group_entry(ret->materials, "material_plasma_red")->element;
    p1->plasma2->status |= OBJECT_TRANSPARENT;
    p1->plasma2->pos.x = -21;
    p1->plasma2->pos.z =  10;
    
    
    Player *p2 = new Player();
    p2->racket->mesh = (mesh3d_t*) oxygarum_get_group_entry(ret->meshes, "racket")->element;
    p2->racket->material = (material_t*) oxygarum_get_group_entry(ret->materials, "material_racket_blue")->element;
    p2->racket->pos.x = 20;
    p2->racket->rot.y = 90;
    
    p2->plasma1->mesh = (mesh3d_t*)oxygarum_get_group_entry(ret->meshes, "plasma")->element;
    p2->plasma1->material = (material_t*)oxygarum_get_group_entry(ret->materials, "material_plasma_blue")->element;
    p2->plasma1->status |= OBJECT_TRANSPARENT;
    p2->plasma1->pos.x = 21;
    p2->plasma1->pos.z = -10;
    
    p2->plasma2->mesh = (mesh3d_t*)oxygarum_get_group_entry(ret->meshes, "plasma")->element;
    p2->plasma2->material = (material_t*)oxygarum_get_group_entry(ret->materials, "material_plasma_blue")->element;
    p2->plasma2->status |= OBJECT_TRANSPARENT;
    p2->plasma2->pos.x = 21;
    p2->plasma2->pos.z =  10;

    
    oxygarum_group_add(scene->objects3d, ball->object, NULL);
    oxygarum_group_add(scene->objects3d, p1->racket, NULL);
    oxygarum_group_add(scene->objects3d, p1->plasma1, NULL);
    oxygarum_group_add(scene->objects3d, p1->plasma2, NULL);
    oxygarum_group_add(scene->objects3d, p2->racket, NULL);
    oxygarum_group_add(scene->objects3d, p2->plasma1, NULL);
    oxygarum_group_add(scene->objects3d, p2->plasma2, NULL);
    
    // main loop
    while (1) {
        // update (calculate frametime, handle events, etc.)
        float frametime = oxygarum_update();

        ball->update(frametime);
        
        // render
        oxygarum_render_screen(screen);
    }

    return 0;
}
