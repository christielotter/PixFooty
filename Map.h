#ifndef MAP_H
#define MAP_H

#include "mbed.h"
#include "N5110.h"
#include "Utils.h"  // for Position


/** Ball Class
@author Dr Craig A. Evans, University of Leeds
@brief Controls the ball in the Pong game 
@date Febraury 2017
*/ 

class Map {
public:
    Map();
    void draw(N5110 &lcd);  // Pass in the lcd object to draw
    void load_user_score();
    void load_dec_score();
    void load_pix_footy();
    const int *player_score[9];
    const int *dec_score[9];
    const int *open_screen[0];
private:



};
#endif