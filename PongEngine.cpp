#include "PongEngine.h"
#include <ios>
DigitalIn buttonKick(A1);
DigitalIn buttonJump(A0);
PongEngine::PongEngine(){ user_goals = 0; dec_goals = 0;}    
bool _goal_scored = false;
Timer _goal_timer;
void PongEngine::init(int paddle_position, int paddle_height, int paddle_width, int ball_size, int speed){
    //printf("Pong Engine: Init\n");
    _ball.init(ball_size,speed);
    _paddle.init(paddle_position, paddle_height, paddle_width);
    _paddle.load_kick_frames();  // you can safely call this here

}

int PongEngine::update(UserInput input, N5110 &lcd) {   
    //printf("Pong Engine: Update\n");
    check_goal(lcd);  // checking for a goal is a priority 
    _ball.update();
    _paddle.update(input);

   
    Position2D paddle_velocity = _paddle.get_velocity();
    Position2D paddle_pos = _paddle.get_pos();
    // important to update paddles and ball before checking collisions so can
    // correct for it before updating the display
    check_wall_collision();
    check_player_collision();
    check_goal_collision();
    player_jump();
    if (buttonKick.read() && !_kick_animation_active) {
        _kick_animation_active = true;
        _paddle.reset_kick();
    }

    if (_kick_animation_active) {
        _paddle.update_kick();

        if (_paddle.get_current_kick_frame() >= 2) {
            _kick_animation_active = false;
        }
    }
    if (buttonJump.read() && (fabs(paddle_velocity.y) >0)) {
        _player_jump = false;
    }else if (buttonJump.read() && !_player_jump){
        _player_jump = true;
    }

    
    return user_goals;
}


void PongEngine::draw(N5110 &lcd) {

    Position2D ball_pos = _ball.get_pos();
    int size = _ball.get_size();
    int speed = abs(_ball.get_velocity().x);  
    _map.draw(lcd);
    _dec.draw(lcd);
      // Draw the appropriate score sprite based on user_goals
    if (user_goals >= 0 && user_goals <= 8 && _goal_scored == false) {
        lcd.drawSprite(19, 0, 28, 28, (int*)_map.player_score[user_goals]);
    }
    if (dec_goals >= 0 && dec_goals <= 8 && _goal_scored == false) {
        lcd.drawSprite(47, 0, 19, 20, (int*)_map.dec_score[dec_goals]);
    }
    _ball.draw(lcd);
   // _paddle.draw(lcd);
     if (_kick_animation_active) {
        _paddle.draw_kick_frame(lcd);
    } else {
        _paddle.draw(lcd);
    }
    // _paddle.draw(lcd);
     if (_kick_animation_active) {
        _paddle.draw_kick_frame(lcd);
    } else {
        _paddle.draw(lcd);
    }
 
    // If a goal is scored, display "GOAL!" and wait 2 seconds before resetting
    if (_goal_scored) {
        lcd.printString(" GOAAAAAAAAAL! ", 0, 1);  // row 1 for goal message
     printf("Goal scored. Timer: %d ms\n", _goal_timer.read_ms());  // Debug print to check timer
        // Wait for 2 seconds before resetting the ball and continuing
        if (_goal_timer.read_ms() > 2000) {
            // Reset ball and paddle after delay
            _goal_scored = false;  // Hide goal message
            _goal_timer.stop();  // Stop timer
        }
    }
}

void PongEngine::check_wall_collision() {
    //printf("Pong Engine: Check Wall Collision\n");
    // read current ball attributes
    Position2D ball_pos = _ball.get_pos();
    Position2D ball_velocity = _ball.get_velocity();
    int size = _ball.get_size();
    const float damping = 0.9;  // Damping factor for bounce (reduce velocity)
    float min_bounce_speed = 2.0f;
    int crossbar_x = 74;
    int crossbar_y = 19;

    int ball_left = ball_pos.x;
    int ball_right = ball_pos.x + size;
    int ball_top = ball_pos.y;
    int ball_bottom = ball_pos.y + size;

    // check if hit roof
    if (ball_top <= 1) {  //  1 due to 1 pixel boundary
        ball_pos.y = 1;  // bounce off ceiling without going off screen
        ball_velocity.y = -ball_velocity.y;  // flip velocity
        //check if hit ground
    } else if (ball_bottom >= 37) {
        ball_pos.y = 37 - size;
        if (fabs(ball_velocity.y) > min_bounce_speed) {
            ball_velocity.y = -ball_velocity.y * damping;
            } else {
            ball_velocity.y = 0;  // stop bouncing
        }
    //check if hit right wall
    }
    if ((ball_right >= WIDTH) && (crossbar_y >= ball_bottom)) {
    ball_pos.x = WIDTH - size;
    ball_velocity.x = -ball_velocity.x;
    }else if ((ball_left <= 1 ) && (crossbar_y >= ball_bottom)) {
        // hit left wall
        ball_pos.x = 1 + size ;  // stops ball going off screen
        ball_velocity.x = -ball_velocity.x ;    // flip velcoity 
    }
    
    // update ball parameters
    _ball.set_velocity(ball_velocity);
    _ball.set_pos(ball_pos);
}
void PongEngine::player_jump(){
    Position2D paddle_pos = _paddle.get_pos();
    Position2D paddle_velocity = _paddle.get_velocity();
    if(_player_jump == true){
        paddle_velocity.y = -10;
        _player_jump = false;
    }
    

// update ball parameters
    _paddle.set_velocity(paddle_velocity);
    _paddle.set_pos(paddle_pos);

}
void PongEngine::check_player_collision() {
    Position2D ball_pos = _ball.get_pos();
    Position2D ball_velocity = _ball.get_velocity();
    Position2D paddle_pos = _paddle.get_pos();
    Position2D paddle_velocity = _paddle.get_velocity();
    int paddle_height = _paddle.get_height();
    int paddle_width = _paddle.get_width();
    int ball_size = _ball.get_size();
    
    int player_left = paddle_pos.x;
    int player_right = paddle_pos.x + paddle_width;
    int player_top = paddle_pos.y;
    int player_bottom = paddle_pos.y + paddle_height;
    
    int ball_left = ball_pos.x;
    int ball_right = ball_pos.x + ball_size;
    int ball_top = ball_pos.y;
    int ball_bottom = ball_pos.y + ball_size;
    
    float player_damping = 0.2;
    
    // Define the kick area when kick animation is active
    if (_kick_animation_active) {
        // Kick extends 5 pixels from the right side of the player
        int kick_left = player_right;
        int kick_right = player_right + 5;
        
        // Check if ball overlaps with kick area
        bool kick_x_overlap = (ball_left < kick_right) && (ball_right > kick_left);
        bool kick_y_overlap = (ball_top < player_bottom) && (ball_bottom > player_top);
        
        if (kick_x_overlap && kick_y_overlap) {
            // Calculate penetration depth (how deep the ball is in the kick area)
            float x_penetration = std::min(kick_right - ball_left, ball_right - kick_left);
            
            // Apply different kick behaviors based on penetration depth
            if (x_penetration <= 1.0f) {
                // 1 pixel overlap: roll ball along floor
                ball_velocity.x = 10.0f;  // Strong forward momentum
                ball_velocity.y = 0.0f;   // No vertical movement
                
            } else if (x_penetration <= 3.0f) {
                // 2-3 pixels overlap: equal x and y velocity
                ball_velocity.x = 8.0f;
                ball_velocity.y = -10.0f;  // Negative for upward movement
                
            } else {
                // 4-6 pixels overlap: more y velocity than x
                ball_velocity.x = 5.0f;
                ball_velocity.y = -18.0f; // Strong upward movement
            }
            
            // Apply the new velocity to the ball
            _ball.set_velocity(ball_velocity);
            
            // Move the ball outside the kick area to prevent multiple kicks
            ball_pos.x = kick_right + 1;
            _ball.set_pos(ball_pos);
            
            // Prevent multiple kicks in the same animation
            _ball_kicked = true;
            return; // Skip regular collision check after kick
        }
    }
    
    // Reset kick flag when animation ends
    if (!_kick_animation_active) {
        _ball_kicked = false;
    }
    
    // Regular player collision check
    bool x_overlap = (ball_left < player_right) && (ball_right > player_left);
    bool y_overlap = (ball_top < player_bottom) && (ball_bottom > player_top);
    
    // Handle regular player collision if not already kicked
    if (x_overlap && y_overlap && !_ball_kicked) {
        // Collision happened, figure out direction
        int overlap_left   = ball_right - player_left;
        int overlap_right  = player_right - ball_left;
        int overlap_top    = ball_bottom - player_top;
        int overlap_bottom = player_bottom - ball_top;
        
        // Find the smallest overlap — that's the side it hit
        int min_overlap = std::min({overlap_left, overlap_right, overlap_top, overlap_bottom});
        
        if (min_overlap == overlap_top) {
            // Hit top of paddle → bounce up
            ball_pos.y = player_top - ball_size;
            ball_velocity.y = -fabs(ball_velocity.y);
        } else if (min_overlap == overlap_bottom) {
            // Hit bottom of paddle → bounce down
            ball_pos.y = player_bottom;
            ball_velocity.y = fabs(ball_velocity.y);
        } else if (min_overlap == overlap_left) {
            if (paddle_velocity.x > 0) {
                ball_pos.x = player_left - ball_size;
                ball_velocity.x = -fabs(ball_velocity.x) * player_damping;
            } else if (paddle_velocity.x < 0) {
                ball_pos.x = player_left - ball_size;
                ball_velocity.x = -fabs(ball_velocity.x) - 2;
            } else {
                // Hit left side → bounce left
                ball_pos.x = player_left - ball_size;
                ball_velocity.x = -fabs(ball_velocity.x) * 0.6;
            }
        } else if (min_overlap == overlap_right) {
            if (paddle_velocity.x > 0) {
                // Hit right side → bounce right
                ball_pos.x = player_right;
                ball_velocity.x = fabs(ball_velocity.x) + 2;
            } else if (paddle_velocity.x < 0) {
                // Hit right side → bounce right
                ball_pos.x = player_right;
                ball_velocity.x = fabs(ball_velocity.x) * player_damping;
            } else {
                ball_pos.x = player_right;
                ball_velocity.x = fabs(ball_velocity.x) * 0.6;
            }
        }
        
        _ball.set_velocity(ball_velocity);
        _ball.set_pos(ball_pos);
    }
}

void PongEngine::check_goal_collision() {
    //printf("Pong Engine: Check Wall Collision\n");
    // read current ball attributes
    Position2D ball_pos = _ball.get_pos();
    Position2D ball_velocity = _ball.get_velocity();
    int size = _ball.get_size();
    float ball_center_x = ball_pos.x + size / 2.0f;
    float ball_center_y = ball_pos.y + size / 2.0f;

    int crossbar_x1 = 10;
    int crossbar_y = 19;

    int crossbar_x2 = 74;
    

    int ball_left = ball_pos.x;
    int ball_right = ball_pos.x + size;
    int ball_top = ball_pos.y;
    int ball_bottom = ball_pos.y + size;
    bool hit1 = (crossbar_x1 <= ball_right && crossbar_x1 > ball_left &&
            crossbar_y >= ball_top && crossbar_y < ball_bottom);
    //Crossbar and goal
    if (hit1) {

    // How far the ball's center is from the crossbar
    float dx1 = fabs(ball_center_x - crossbar_x2);
    float dy1 = fabs(ball_center_y - crossbar_y);

    if (dy1 > dx1) {
        // Top or bottom hit → bounce Y
        ball_velocity.y = -ball_velocity.y *0.95;
    } else {
        // Left or right hit → bounce X
        ball_velocity.x = -ball_velocity.x * 0.95;
    }
    }
    //Top of goal
    bool top_of_goal1 = (crossbar_x1 >= ball_right && 1 <= ball_left &&
            crossbar_y >= ball_top && crossbar_y <= ball_bottom);
    if (top_of_goal1){
    ball_bottom = crossbar_y + 1;
    ball_velocity.y = -ball_velocity.y;
    }

    bool hit2 = (crossbar_x2 >= ball_left && crossbar_x2 < ball_right &&
            crossbar_y >= ball_top && crossbar_y < ball_bottom);
    //Crossbar and goal
    if (hit2) {
 // How far the ball's center is from the crossbar
    float dx2 = fabs(ball_center_x - crossbar_x2);
    float dy2 = fabs(ball_center_y - crossbar_y);

    if (dy2 > dx2) {
        // Top or bottom hit → bounce Y
        ball_velocity.y = -ball_velocity.y *0.95;
    } else {
        // Left or right hit → bounce X
        ball_velocity.x = -ball_velocity.x * 0.95;
    }
    }
    //Top of goal
    bool top_of_goal2 = (crossbar_x2 <= ball_left && WIDTH >= ball_right &&
            crossbar_y >= ball_top && crossbar_y <= ball_bottom);
    if (top_of_goal2){
    ball_bottom = crossbar_y + 1;
    ball_velocity.y = -ball_velocity.y;
    }
    // write new attributes
    _ball.set_velocity(ball_velocity);
    _ball.set_pos(ball_pos);
}

void PongEngine::check_goal(N5110 &lcd) {
    //printf("Pong Engine: Check Goal\n");
    Position2D ball_pos = _ball.get_pos();
    Position2D paddle_pos = _paddle.get_pos();
    int height = _paddle.get_height();
    int width = _paddle.get_width();
    int goalline_x1 = 10;
    int goalline_x2 = 74;
    int crossbar_y = 19;
    int size = _ball.get_size();
    int ball_left = ball_pos.x;
    int ball_right = ball_pos.x + size;
    int ball_top = ball_pos.y;
    int ball_bottom = ball_pos.y + size;
   
    int speed = abs(_ball.get_velocity().x);  // speed is magnitude of velocity
    // check if ball position has gone off the left
    if (
        (ball_top > crossbar_y)&&
        (ball_left > goalline_x2)) {
        _goal_scored = true;
        user_goals++;
        _ball.set_velocity({0, 0});  // Stop the ball after the goal
        _ball.init(size, 0);
        _paddle.init(24 - width, height, width);
        _goal_timer.stop();
        _goal_timer.reset();
        _goal_timer.start();
    }
    if (
        (ball_top > crossbar_y)&&
        (ball_right < goalline_x1)) {
        _goal_scored = true;
        dec_goals++;
        _ball.set_velocity({0, 0});  // Stop the ball after the goal
        _ball.init(size, 0);
        _paddle.init(24 - width, height, width);
        _goal_timer.stop();
        _goal_timer.reset();
        _goal_timer.start();
    }
    }   

