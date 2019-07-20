#ifndef BALL_H
#define BALL_H

#include "court.h"
class ball {
public:
    ball(vec3 p) : pos(p) {}
    vec3 pos, vel{ 0, 0, 0 }, init_vel{ 0, 0, 0 }, accel{ 0, 0, -9.8 };
    int lastHit = 0;//nobody hit it last
    float speed = 0, heading = 0;
    int num_bounces = 0;
    vec3 hitPos;
    bool died = false, isReset = false;;//has the ball died or not
    int num_points_played = 0;
    int num_hits = 0;
    int num_good_hits = 0;
    float dead_frames_max = 100 / mult;
    int isDead_frames = dead_frames_max+1;//(like net (court)) # of frames for 'dead' ball animation before reset
    int point_p1 = 0, game_p1 = 0, set_p1 = 0;
    int point_p2 = 0, game_p2 = 0, set_p2 = 0;
    vec3 initPos_p1{ c_w / 2, c_h, 1 }, initPos_p2{ c_w / 2, 0, 1 };
    void reset();
    void reset_rand();
    void updateFrames();
    void update(court* c, float dt = 60);
    void output_data(/*player* p1, player* p2*/);
    void dead(bool flipWinner = false);
    bool isOut(court* c);
    void net(court* c);
    void bounce(const float eff, court* c, const float dt);
    void draw();
};
#endif // BALL_H
