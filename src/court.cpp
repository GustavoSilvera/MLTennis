#include "court.h"
vec2 court::randomPoint() {//generates some random point in the no-mans-zone
    //less random
    float x = ((rand() % 100) / 100.0) * c_w;
    float y = ((rand() % 100) / 100.0) * mid_c_h * 0.75;
    //more random
    //float x = ((rand() % 100) / 100.0) * (2*c_w) - c_w/2;
    //float y = ((rand() % 100) / 100.0) * (mid_c_h * 1.5)  - mid_c_h/2;

    randPt = vec2{ x, y };
    return(randPt);
}
void court::updateFrames() {
    net_frames_max = 10 / mult;
    isNet_frames = net_frames_max;
}
void court::bounce() {
    ofSetColor(0, 255, 0);//green = in
    if (ball_pos.x < c_w - alley && ball_pos.x > alley) {//within singles court
        //no mans land on upper side
        if (ball_pos.y < base_serv) {
            //ofDrawRectangle(ofRectangle(vec2{}, vec2{}));
            ofDrawRectangle(ofRectangle(ppm * vec2(alley, 0), ppm * vec2(c_w - alley, base_serv)));
        }
        //service boxes on upper side
        else if (ball_pos.y < mid_c_h) {//servus boxes
            if (ball_pos.x < mid_c_w) {
                ofDrawRectangle(ofRectangle(ppm * vec2(alley, base_serv), ppm * vec2(mid_c_w, mid_c_h)));
            }
            else {
                ofDrawRectangle(ofRectangle(ppm * vec2(mid_c_w, base_serv), ppm * vec2(c_w - alley, mid_c_h)));
            }
        }
        //servus boxes on lower side
        else if (ball_pos.y < c_h - base_serv) {
            if (ball_pos.x < mid_c_w) {
                ofDrawRectangle(ofRectangle(ppm * vec2(alley, mid_c_h), ppm * vec2(mid_c_w, c_h - base_serv)));
            }
            else {
                ofDrawRectangle(ofRectangle(ppm * vec2(mid_c_w, mid_c_h), ppm * vec2(c_w - alley, c_h - base_serv)));
            }
        }
        //no-mans-land on lower side
        else if (ball_pos.y < c_h) {
            ofDrawRectangle(ofRectangle(ppm * vec2(alley, c_h - base_serv), ppm * vec2(c_w - alley, c_h)));
        }
    }
    else if (ball_pos.y > 0 && ball_pos.y < c_h) {//within the court
        ofSetColor(255, 0, 0);//wide(alley) = bad = red
        if (ball_pos.x > 0 && ball_pos.x < alley) {//left alley
            if (ball_pos.y < mid_c_h) {
                ofDrawRectangle(ofRectangle(ppm * vec2(0, 0), ppm * vec2(alley, mid_c_h)));
            }
            else {
                ofDrawRectangle(ofRectangle(ppm * vec2(0, mid_c_h), ppm * vec2(alley, c_h)));
            }
        }
        else if (ball_pos.x > c_w - alley && ball_pos.x < c_w) {//right alley
            if (ball_pos.y < mid_c_h) {
                ofDrawRectangle(ofRectangle(ppm * vec2(c_w - alley, 0), ppm * vec2(c_w, mid_c_h)));
            }
            else {
                ofDrawRectangle(ofRectangle(ppm * vec2(c_w - alley, mid_c_h), ppm * vec2(c_w, c_h)));
            }
        }
    }
    ball_bounced = false;//not bouncing anymore
}
void court::draw() {//all in meters
    ofSetColor(30.0 , 94.0 , 255); //RGB: (30, 94, 255) ~ tennis blue
    ofDrawRectangle(ofRectangle(ppm * vec2(0, 0), ppm * vec2(c_w, c_h)));
    if (ball_bounced) bounce();
    ofSetColor(255, 255, 255);//resetColour
    ofNoFill();
    ofDrawRectangle(ofRectangle(ppm * vec2(0, 0), ppm * vec2(c_w, c_h)));//outer(doubles side & base) lines
    ofDrawRectangle(ofRectangle(ppm * vec2(0, base_serv), ppm * vec2(c_w, c_h - base_serv)));//service lines
    ofDrawRectangle(ofRectangle(ppm * vec2(alley, 0), ppm * vec2(c_w - alley, c_h)));//singles side lines
    ofFill();
    ofDrawLine(ppm * vec2(mid_c_w, base_serv), ppm * vec2(mid_c_w, c_h - base_serv));//center service line
    ofDrawLine(ppm * vec2(mid_c_w, 0), ppm * vec2(mid_c_w, 0.5));//center 'tee's
    ofDrawLine(ppm * vec2(mid_c_w, c_h), ppm * vec2(mid_c_w, c_h - 0.5));//center 'tee's
    if (isNet_frames < net_frames_max + 1) {//at least one frame
        ofSetColor(255, 0, 0);
        isNet_frames++;
    }
    ofDrawLine(ppm * vec2(0, mid_c_h), ppm * vec2(c_w, mid_c_h));//NET line
}
