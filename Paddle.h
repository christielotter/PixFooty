#ifndef PADDLE_H
#define PADDLE_H

#include "mbed.h"
#include "N5110.h"
#include "Utils.h"

class Paddle
{
public:

    Paddle();
    void init(int x,int height,int width);
    void draw(N5110 &lcd);
    void update(UserInput input);
    void add_score();
    int get_score();
    Position2D get_pos();
    int get_height();
    int get_width();
    void set_velocity(Position2D v);
    void set_pos(Position2D p);
    Position2D get_velocity();
    //Kicking
    void load_kick_frames();
   // void play_kick_animation(N5110 &lcd);
    void reset_kick();
    void update_kick();
    void draw_kick_frame(N5110 &lcd);
    int get_current_kick_frame();

private:

    int _height;
    int _width;
    int _x;
    int _y;
    int _speed;
    int _score;
    const int *_kick_frames[2];
    int _current_kick_frame;
    Timer _kick_frame_timer;

    Position2D _velocity;

};
#endif