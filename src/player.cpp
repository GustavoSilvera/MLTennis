#include "player.h"

bool player::withinRange(ball* b) {//if b->pos is within 1m of pos
    //return(pythag(b->pos.x - pos.x, b->pos.y - pos.y) < 1.6);//no sqrt optimization
    return(pythag2(b->pos.x - pos.x, b->pos.y - pos.y, b->pos.z - pos.z) < sqr(1.3));//w/ sqrt opt
}
void player::hit(ball* b, float initV, float aim) {
    aim = toRad(aim);//converts to radians
    //to be less random just change the rand() to 3
    b->init_vel = { initV * cos(aim), initV * -sin(aim), (float)((rand() % 100) / 100.0) + (float)2.5 };//new initial velocity
    b->vel = b->init_vel;
    b->heading = toDeg(aim);
    b->isReset = false;
    if (b->speed == 0) b->died = false;//ball is 'alive' again (after being dead) when somebody hits it (AFTER BEING RESET)
    if (b->num_bounces == 0) b->num_good_hits++;//volley (still good)
    if (!b->died) b->num_hits++;//num hits increases
    b->num_bounces = 0;
}
void player::hit(ball* b, vec3 initV) {//hit with certain velocity components
    b->init_vel = initV;//new initial velocity
    b->vel = b->init_vel;
    b->heading = toDeg(atan2(-initV.y, initV.x));
    b->isReset = false;
    if (b->speed == 0) b->died = false;//ball is 'alive' again (after being dead) when somebody hits it (AFTER BEING RESET)
    if (!b->died) b->num_hits++;//num hits increases
    if (b->num_bounces == 0) b->num_good_hits++;//volley (still good)
    b->num_bounces = 0;
}
vec2 player::target(court* c) {//calculate the best direction that is furthest from the other player (but still in)
    return refPt(identity) - /*dir * vec2{10, 0}*/ altSign * c->randomPoint();
    //just a random point for now (in reference to corners [equality])
}
float player::targetDirection(vec2 target) {
    return atan2(pos.y - target.y, target.x - pos.x);//returns radians of optimal point
}
bool player::closeEnough(vec3 p, vec3 t, float thresh) {
    return(pythag2(p.x - t.x, p.y - t.y) < sqr(thresh));
}
void player::retreat(float speed) {
    vel.x = speed * ((c_w / 2) - pos.x);//return to center FAST
    vel.y = speed * (refPt(identity - 1).y - pos.y);//return to respective center FAST
    //using identity-1 to SWAP baseline y-coordinates from the refPt code above
}
void player::moveTowards(vec2 p, float vel_x_mult, float vel_y_mult) {
    vel.x = vel_x_mult * clamp(-3, 3, p.x - pos.x);//faster in x direction (side stepping)
    vel.y = vel_y_mult * clamp(-3, 3, p.y - pos.y);
}
void player::move(ball* b, player* o, int dt) {
    if (b->died && !closeEnough(pos, vec3{ c_w / 2, 0, 0 }, 0.2)) {//if ball is dead and player has yet to reach the center
        retreat(2);//return to center FAST
    }
    else if (b->lastHit != identity && b->speed != 0) {//if chasing the ball (hasnt hit it yet)
        //prediction of where the ball will land based off speed & angle
        float hypot = altSign *  (c_h) / sin(toRad(b->heading));//gets distance from ball to baseline (constant y point)
        float predictX = (b->hitPos.x + hypot * cos(toRad(b->heading)));
        float average_x_prediction = (predictX + o->rel_placement.x) / 2;
        //moveTowards(vec2(average_x_prediction, o->rel_placement.y), 3, 1);

        moveTowards(vec2(predictX, b->pos.y), 3, 0.5);
    }
    else {
        retreat();//return to center
    }
}
std::vector<double> player::add_rand(std::vector<double> orig, double range, int exclude) {
    std::vector<double> randomized;
    for (int i = 0; i < orig.size(); i++) {
        if (i != exclude) {//dont want to modify Z for instance
            const int t_range = range * 200;
            double random = (rand() % t_range) / 100 - range;//positive and negative double of range with 2 decimal places (100)
            randomized.push_back(orig[i] + random);
        }
        else randomized.push_back(orig[i]);
    }
    return randomized;
}
void player::update(ball* b, court* c, player* o, ConvNeuralNet* cnn, float dt) {//refresh 60fps
    pos += vel / dt;
    vel += accel / dt;
    if (withinRange(b) && b->lastHit != identity) {
        /*rel_placement = target(c);
            float initVel = (rand() % 500) / 100 + 18;//from 5 to 30 random
                                                       //float initVel = pythag(rel_placement.x - pos.x, rel_placement.y - pos.y);
            hit(b, initVel, toDeg(targetDirection(rel_placement)));//ball, initial velocity, direction
            */
        std::vector<double> p = { b->pos.x, b->pos.y, b->pos.z };
        if (identity == 1) {//player one's position relative to origin corner(bottom right)
            p[1] = c_h - p[1];//y
            p[0] = c_w - p[0];//x
        }
        int dir = 1;
        if (identity != 1) {//player 2 hit
            dir *= -1;
        }
        std::vector<double> hit_pos = add_rand(p, 1.5, 2);//add randomness range of 1.5m
        std::vector<double> v = add_rand(cnn->compute(hit_pos), 1.5, 2);//MACHINE LEARNIN'''''
        vec3 net_vel = { dir*v[0], -dir*1.5*v[1], v[2] + 1 };//issok

        hit(b, net_vel);
        b->lastHit = identity;//updates lastHit identity
        b->hitPos = b->pos;//updates last_hit position
    }
    heading = toDeg(atan2(pos.y - b->pos.y, pos.x - b->pos.x)) - 90;//always point towards the ball
    move(b, o, dt);
    if ((b->lastHit != identity) && b->isReset) {
        pos.x = b->pos.x;
        pos.y = b->pos.y;
    }
}
void player::drawCross(vec2 p, float length) {//x marks the spot
    ofDrawLine(ppm * vec2(p.x - length / 2, p.y - length / 2), ppm * vec2(p.x + length / 2, p.y + length / 2));
    ofDrawLine(ppm * vec2(p.x - length / 2, p.y + length / 2), ppm * vec2(p.x + length / 2, p.y - length / 2));
}
void player::draw() {
    glPushMatrix();//for rotation
    if (identity == 1) ofSetColor(0, 255, 0);
    else ofSetColor(255, 0, 0);
    drawCross(rel_placement, 1);//draws points for target
    ofTranslate(ppm * pos);
    ofSetColor(colour.x, colour.y , colour.z );
    ofRotateDeg(heading);//3rd input used for 2D rotation (angle)
    const vec2 center = { 0.5, 0.5 };//away from position (to have it centered)
    const vec2 pos2D = { pos.x, pos.y };
    ofFill();
    ofDrawRectangle(ofRectangle(ppm * (-center), ppm * (center)));
    ofSetColor(255, 255, 255);
    ofDrawLine(ppm * (-center), ppm * (vec2{ 0.5, -0.5 }));//indicates heading edge
    ofDrawLine(ppm * (-vec2{ 0, 1 }), (vec2{ 0, 0 }));//indicates heading edge
    glPopMatrix();
}
