#include "Ball.h"

Ball::Ball() {}
const float acc = 0.2f;
    float dt = 0.1;
void Ball::init(int size, int speed) {
    //printf("Ball: Init\n");
    _size = size;
    _x = WIDTH/2 -  _size/2;
    _y = HEIGHT/4 - _size/2;

    _velocity.x = speed;
    _velocity.y = speed;
}
const int ball [6][6]{
{0, 1, 1, 1, 1, 0},
{1, 1, 0, 0, 1, 1},
{1, 0, 1, 1, 0, 1}, 
{1, 0, 1, 1, 0, 1},
{1, 1, 0, 0, 1, 1},
{0, 1, 1, 1, 1, 0},
};
void Ball::draw(N5110 &lcd) {
    //printf("Ball: Draw\n");
    lcd.drawSprite((int)_x,(int)_y,6,6,(int*)ball);
}

void Ball::update() {
    const float acc = 9.81f;  // gravity
    float dt = 0.25f;         // time step

    // 1. Update velocity with acceleration
    _velocity.y = _velocity.y + acc * dt;  // v = v₀ + a*t
if (_velocity.y > -0.5f && _velocity.y <= 0.0f) {
    _velocity.y = 1;
}
    // 2. Update position using updated velocity
    _x += _velocity.x;
    _y += _velocity.y * dt + 0.5 * acc * dt * dt;  // y = y₀ + v₀*t + ½*a*t²
}


void Ball::set_velocity(Position2D v){
    printf("Ball: Velocity -> x: %.2f, y: %.2f\n", v.x, v.y);
     
    _velocity.y = v.y;
    _velocity.x = v.x;
}

void Ball::set_pos(Position2D p) {
    printf("Ball: Set Position\n");
    _x = p.x;
    _y = p.y;
}

Position2D Ball::get_velocity(){ return {_velocity.x,_velocity.y}; }

Position2D Ball::get_pos() { return {_x,_y}; }

int Ball::get_size() { return _size; }

