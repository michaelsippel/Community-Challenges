/*
 * File:   Ball.cpp
 * Author: micha
 *
 * Created on 1. April 2014, 17:44
 */

#include "Ball.h"
#include "Player.h"
#include <oxygarum.h>
#include <math.h>

extern Player *p1;
extern Player *p2;

extern float time;
extern char s_middle[];
extern text_t *t_middle;

Ball::Ball() {
    this->object = oxygarum_create_object3d();
    this->pos.x = 0;
    this->pos.y = 0;
}

Ball::Ball(const Ball& orig) {
}

Ball::~Ball() {
}

void Ball::update(float speed) {
    if (this->follow == NULL) {
        // move
        this->pos.x += this->velocity.x * speed;
        this->pos.y += this->velocity.y * speed;

        // normalize velocity
        if (this->velocity.x > 0.0f) {
            if (this->velocity.x < 0.05) {
                this->velocity.x += 0.002 * speed;
            } else if (this->velocity.x > 0.08) {
                this->velocity.x -= 0.002 * speed;
            }
        } else if (this->velocity.x < 0.0f) {
            if (this->velocity.x < -0.08f) {
                this->velocity.x += 0.002 * speed;
            } else if (this->velocity.x > -0.05) {
                this->velocity.x -= 0.002 * speed;
            }
        }

        // check collisions
        if (p1->check_collision(this->get_pos())) {
            this->velocity.x = fabs(this->velocity.x);
        } else if (this->pos.x < -30.0f) {
            this->velocity.x = fabs(this->velocity.x);
            p1->use_power(-30.0f);
        }

        if (p2->check_collision(this->get_pos())) {
            this->velocity.x = -fabs(this->velocity.x);
        } else if (this->pos.x > 30.0f) {
            this->velocity.x = -fabs(this->velocity.x);
            p2->use_power(-30.0f);
        }

        if (this->pos.y > 15.0f) {
            this->velocity.y = -fabs(this->velocity.y);
        } else if (this->pos.y < -15.0f) {
            this->velocity.y = fabs(this->velocity.y);
        }
    } else {
        if (follow->racket->pos.x > 0.0f) {
            this->pos.x = 18.0f;
        } else {
            this->pos.x = -18.0f;
        }
        this->pos.y = follow->racket->pos.z;
    }

    this->object->pos.x = this->pos.x;
    this->object->pos.z = this->pos.y;
}

vertex2d_t Ball::get_pos() {
    return this->pos;
}

Player *Ball::get_player() {
    return this->follow;
}

void Ball::reset(Player *p) {
    this->follow = p;
    this->velocity.x = 0;
    this->velocity.y = 0;
}

void Ball::launch(Player *p) {
    if (p->racket->pos.x <= this->pos.x && this->pos.x < -10.0f) {
        this->velocity.x += 0.3f;
        p->use_power(-15.0f);
        this->velocity.y = (float) (rand() & 0xf) * 0.003;
        this->follow = NULL;
        sprintf(s_middle, "POWER!");
        time = 1000.0f;
        t_middle->color.rgb.r = 1;
        t_middle->color.rgb.g = 0;
        t_middle->color.rgb.b = 0;
    } else if (p->racket->pos.x >= this->pos.x && this->pos.x > 10.0f) {
        this->velocity.x -= 0.3f;
        p->use_power(-15.0f);
        this->velocity.y = (float) (rand() & 0xf) * 0.003;
        this->follow = NULL;
        sprintf(s_middle, "POWER!");
        time = 1000.0f;
        t_middle->color.rgb.r = 0;
        t_middle->color.rgb.g = 0;
        t_middle->color.rgb.b = 1;
    }
}

void Ball::stop(Player *p) {
    if (p->racket->pos.x <= this->pos.x && this->pos.x < -10.0f ||
            p->racket->pos.x >= this->pos.x && this->pos.x > 10.0f) {
        this->velocity.x *= 0.8;
        p->use_power(-2.0f);
    }
}
