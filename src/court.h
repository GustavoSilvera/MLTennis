#ifndef COURT_H
#define COURT_H
#include "util.h"
#include "neuralnet.h"

class court {
public:
    const double mid_c_w = c_w / 2, mid_c_h = c_h / 2;
    const double base_serv = 5.5;//distance from baseline to service line
    const double alley = 1.372;//width of alleys
    const double net_ht = 0.914;//height of net in m
    bool isNet = false;
    bool ball_bounced = false;
    vec3 ball_pos{ 0, 0, 0 };
    double net_frames_max = 10 / mult;
    int isNet_frames = net_frames_max;//no of frames of net animation (when ball hits net)
    vec2 randPt{ 0, 0 };
    vec2 randomPoint();
    void updateFrames();
    void bounce();
    void draw();
};

#endif // COURT_H
