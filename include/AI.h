/* 
 * File:   AI.h
 * Author: micha
 *
 * Created on 3. April 2014, 18:22
 */

#ifndef AI_H
#define	AI_H

#include <oxygarum.h>
#include "Player.h"

class AI {
public:
    AI();
    AI(Player *player);
    AI(Player *player, float strength);
    AI(const AI& orig);
    virtual ~AI();

    void update(vertex2d_t ball_pos);
private:
    Player *player;
    float strength;
    vertex2d_t watched_ball_pos;
    vector2d_t watched_vector;
};

#endif	/* AI_H */

