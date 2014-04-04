/* 
 * File:   AI.cpp
 * Author: micha
 * 
 * Created on 3. April 2014, 18:22
 */

#include "AI.h"

AI::AI() {
}

AI::AI(Player *player) {
    this->player = player;
    this->strength = 0;
}

AI::AI(Player *player, float strength) {
    this->player = player;
    this->strength = strength;
}

AI::AI(const AI& orig) {
}

AI::~AI() {
}

void AI::update(vertex2d_t ball_pos) {
    int m = 0;

    float rpos = this->player->racket->pos.z;

    this->watched_vector.x = watched_ball_pos.x - ball_pos.x;
    this->watched_vector.y = watched_ball_pos.y - ball_pos.y;

    this->watched_ball_pos.x = ball_pos.x;
    this->watched_ball_pos.y = ball_pos.y;

    float dist = 20.0f - this->strength / 5.0f;
    
    if (this->watched_vector.x < 0.0f && ball_pos.x > dist) {
        if (ball_pos.y > rpos + 3.0f) {
            m = 1;
        } else if (ball_pos.y < rpos - 3.0f) {
            m = -1;
        }
    } else if (this->watched_vector.x < 0.0f && ball_pos.x > 20.0f) {
        if (ball_pos.y < rpos + 3.0f) {
            m = -1;
        } else if (ball_pos.y > rpos - 3.0f) {
            m = 1;
        }
    }

    this->player->move(m);
}
