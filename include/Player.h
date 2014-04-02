/* 
 * File:   Player.h
 * Author: micha
 *
 * Created on 1. April 2014, 17:32
 */

#ifndef PLAYER_H
#define	PLAYER_H

#include "Ball.h"
#include <oxygarum.h>

class Player {
public:
    Player();
    Player(const Player& orig);
    virtual ~Player();
    
    int check_collision(Ball *ball);
    object3d_t *racket;
    object3d_t *plasma1;
    object3d_t *plasma2;
private:
    float power;
    int score;
};

#endif	/* PLAYER_H */

