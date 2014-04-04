/* 
 * File:   Ball.h
 * Author: micha
 *
 * Created on 1. April 2014, 17:44
 */

#ifndef BALL_H
#define	BALL_H

#include <oxygarum.h>
#include "Player.h"

class Ball {
public:
    Ball();
    Ball(const Ball& orig);
    virtual ~Ball();
    
    void update(float speed);
    vertex2d_t get_pos();
    void reset(Player *p);
    void launch(Player *p);
    void stop(Player *p);
    
    object3d_t *object;
private:
    Player *follow;
    vertex2d_t pos;
    vector2d_t velocity;
};

#endif	/* BALL_H */

