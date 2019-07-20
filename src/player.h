#ifndef PLAYER_H
#define PLAYER_H

#include "ball.h"

class player {
public:
    player(vec3 p) : pos{ p }, colour{ 255, 255, 255 } {}
    player(vec3 p, float h, vec3 col, int i) : pos{ p }, heading(h), colour{ col }, identity(i) {}
    vec3 pos;
    vec3 colour;
    float heading = 0;
    int identity;//differentiating between p1 and p2
    vec3 vel = { 0, 0, 0 };
    vec3 accel = { 0, 0, 0 };
    bool withinRange(ball* b);
    void hit(ball* b, float initV, float aim = 90);
    void hit(ball* b, vec3 initV);
    vec2 rel_placement{ 0, 0 };
    vec2 target(court* c);
    float targetDirection(vec2 target);
    bool closeEnough(vec3 p, vec3 t, float thresh = 1);
    void retreat(float speed = 1);
    void moveTowards(vec2 p, float vel_x_mult, float vel_y_mult);
    void move(ball* b, player* o, int dt = 60);
    std::vector<double> add_rand(std::vector<double> orig, double range, int exclude = -1);
    void update(ball* b, court* c, player* o, ConvNeuralNet* cnn, float dt = 60);
    void drawCross(vec2 p, float length);
    void draw();
};
#endif // PLAYER_H
