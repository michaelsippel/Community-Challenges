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

int main(int argc, char **argv) {
  // init
  oxygarum_set_resolution(1024, 576);
  oxygarum_set_title("Pong");
  init_oxygarum();
  
  // setup
  screen_t *screen = oxygarum_create_screen();
  scene_t *scene = oxygarum_create_scene();
  screen->scene = scene;
  screen->camera->pos.z = -10.0;
  screen->camera->pos.y = -3.0;
  
  // load mesh and create object
  struct load_return *ret = oxygarum_load_oxy3d_file("data/scene.oxy3d", NULL);
  
  object3d_t *plasma = oxygarum_create_object3d();
  plasma->mesh = (mesh3d_t*) oxygarum_get_group_entry(ret->meshes, "ball")->element;
  plasma->status = OBJECT_VISIBLE | OBJECT_TRANSPARENT;
  oxygarum_group_add(scene->objects3d, plasma, NULL);
  
  // main loop
  while(1) {
    // update (calculate frametime, handle events, etc.)
    float frametime = oxygarum_update();
    
    // render
    oxygarum_render_screen(screen);
  }
  
  return 0;
}
