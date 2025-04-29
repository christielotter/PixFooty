#ifndef PONGENGINE_H
#define PONGENGINE_H

#include "mbed.h"
#include "N5110.h"
#include "Ball.h"
#include "Utils.h"
#include "Paddle.h"
#include "Map.h"
#include "Dec.h"

class PongEngine {
    public:
        PongEngine();  // pass in the lcd object from the main file
        void init(int paddle_position,int paddle_height,int paddle_width,int ball_size,int speed);
        int update(UserInput input, N5110 &lcd);
        void draw(N5110 &lcd);
        int user_goals;
        int dec_goals;
    private:
        void check_wall_collision();
        void check_goal_collision();
        void check_player_collision();
        void check_dec_collision();
        void check_goal(N5110 &lcd);
        void player_jump();
        Ball _ball;
        Paddle _paddle;
        
        Map _map;
        Dec _dec;

        bool _kick_animation_active = false;
        bool _player_jump = false;
        Timer _kick_timer;
        bool _ball_kicked = false;

        
};

#endif