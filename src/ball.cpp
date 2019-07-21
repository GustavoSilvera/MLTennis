#include "ball.h"

void ball::reset() {
    if (lastHit == 1) pos = initPos_p2;
    else pos = initPos_p1;
    vel = { 0, 0, 0 };
    speed = 0;
    isDead_frames = dead_frames_max + 1;//do nothing
}
void ball::reset_rand() {
    isReset = true;
    float x = ((rand() % 100) / 100.0) * (2 * c_w) - c_w / 2;
    float y = ((rand() % 100) / 100.0) * (c_h * 0.75) - c_h / 4;
    vec2 randPt = { x, y };
    vec2 randPt2;
    if (lastHit != 1) randPt2 = refPt2(lastHit) - randPt;
    else randPt2 = refPt2(lastHit) + randPt;
    pos = { randPt2.x, randPt2.y, 1 };
    vel = { 0, 0, 0 };
    speed = 0;
    isDead_frames = dead_frames_max + 1;//do nothing
}
void ball::updateFrames(float m) {
    dead_frames_max = 100.0 / m;
    isDead_frames = dead_frames_max;
}
void ball::update(court* c, float dt) {
    pos += vel / dt;
    vel += accel / dt;
    if (pos.z <= 0) bounce(0.7, c, dt);
    speed = pythag(vel.x, vel.y);
    if (pos.y < c->mid_c_h && (pos.y + (vel.y / dt)) > c->mid_c_h ||    //pos y switches from being less than net to greater than net
            pos.y > c->mid_c_h && (pos.y + (vel.y / dt)) < c->mid_c_h) {	//pos y switches from being greater than net to less than net
        net(c); //checks net conditions
    }
    if (isDead_frames <= dead_frames_max && isDead_frames + 1 > dead_frames_max) {//if animation frames equal
        reset();
        //reset_rand();
    }
    if (num_bounces >= 2) dead(true);//cant have more than 2 bounces
}
void ball::output_data(/*player* p1, player* p2*/) {//DATA COLLECTION YAAAAY
    const int p = 3;//how th numbers should be rounded for the shots to not be so precise
    if (lastHit == 1) {//player one's position relative to origin corner(bottom right)
        DATA << setprecision(p) << c_w - hitPos.x << " "
             << setprecision(p) << c_h - hitPos.y << " ";
    }
    else {
        DATA << setprecision(p) << hitPos.x << " "
             << setprecision(p) << hitPos.y << " ";
    }
    int dir = 1;
    if (lastHit != 1) {//player 2 hit
        dir = -1;
    }
    DATA//continue the segment
            << setprecision(p) << hitPos.z << " "
               //where the ball bounces
               /*<< setprecision(p) << pos.x << " "
                       << setprecision(p) << pos.y << " "*/
               //<< setprecision(p) << pos.z << " "//always goinng to be 0? -- YES
               //components of the shot
            << setprecision(p + 1) << dir*init_vel.x << " "//negative is left, positive is right (RELATIVE TO PLAYER)
            << setprecision(p + 1) << abs(init_vel.y) << " "//only focus on positive for now
            << setprecision(p + 1) << init_vel.z << " \n";
    /*if (lastHit == 1) {
            DATA << "          1 \n";
        }
        else DATA << "         2 \n";*/
    //DATA = std::ofstream("DATA.txt", ofstream::app);
    DATA.flush();
}
void ball::dead(bool flipWinner) {//for instance, p1 hits ball, p2 gets double bounce... p1 SHOULD still get the point even though was last to hit
    if (!died) {//cannot die twice
        isDead_frames = 0;//ball is dead
        died = true;
        num_points_played++;
        if (flipWinner) lastHit -= 1;//flips who wins
        if (lastHit == 1) {//p1 lost the point
            if (game_p2 == 6) { //SHOULD implement the rule that must win by 2, like 7:5 or tiebreaker
                game_p1 = 0;
                game_p2 = 0;
                set_p2 += 1;
                point_p1 = 0;
                point_p2 = 0;
            }
            if (point_p2 < 30) point_p2 += 15;
            else if (point_p2 < 40) point_p2 += 10;
            else { //IMPLEMENT DEUCE (40:40)
                point_p1 = 0;
                point_p2 = 0;
                game_p2 += 1;
            }
        }
        else {//p2 lost the point
            if (game_p1 == 6) { //SHOULD implement the rule that must win by 2, like 7:5 or tiebreaker
                game_p2 = 0;
                game_p1 = 0;
                set_p1 += 1;
                point_p2 = 0;
                point_p1 = 0;
            }
            if (point_p1 < 30) point_p1 += 15;
            else if (point_p1 < 40) point_p1 += 10;
            else { //IMPLEMENT DEUCE (40:40)
                point_p2 = 0;
                point_p1 = 0;
                game_p1 += 1;
            }
        }
        //output_data();
    }
}
bool ball::isOut(court* c) {
    if (pos.x < c->alley || pos.x > c_w - c->alley) return true;//if too WIDE
    if (pos.y < 0 || pos.y > c_h) return true; //if too LONG
    return false;
}
void ball::net(court* c) {
    if (pos.z < c->net_ht && pos.x < c_w && pos.x > 0) {//lower than net but still in front of it
        pos.y = c->mid_c_h;//resets position
        vel.y *= -0.1;//slows down(a lot) and oppo direction
        vel.x *= 0.1;
        c->isNet_frames = 0;//net color animation on court drawing
        dead();
    }
}
void ball::bounce(const float eff, court* c, const float dt) {//bounce off ground w/ certain energy loss & efficiency
    pos.z = 0;
    //slightly lower/slower based off surface material
    vel.z = (vel.z - (accel.z / dt)) * -eff;//gets PREVIOUS velocity (subtract accel) to get realistic bounce reflection (on 100%effic)
    if (num_bounces == 0) {//(first bounce)
        if (isOut(c)) {
            dead(); //if ball bounces OUT
        }
        else if ((lastHit == 1 && pos.y > c_h / 2) ||//from p1, greater than halfCt (before net)
                 (lastHit == 2 && pos.y < c_h / 2)) { //from p2, less than halfCt (before net)
            dead();
        }
        else {
            output_data();
            num_good_hits++;
        }

    }
    c->ball_bounced = true;
    c->ball_pos = pos;
    num_bounces++;
}
void ball::draw() {
    if (isDead_frames >= dead_frames_max) ofSetColor(255, 255, 0);//yellow ball
    else {
        ofSetColor(255, 0, 0);//red (dead ball)
        isDead_frames++;
    }
    ofDrawCircle(ppm * vec2{ pos.x, pos.y }, 3 + 7 * pos.z);
    //gl::drawLine(ppm*Vec2f(pos.x, pos.y), ppm*Vec2f(pos.x + vel.x, pos.y + vel.y));
}
