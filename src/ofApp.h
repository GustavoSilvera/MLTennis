#pragma once

#include "ofMain.h"
#include "player.h"

class ofApp : public ofBaseApp{

public:
    void setup();
    void update();
    void draw();

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void drawScore(ball* b);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    ConvNeuralNet cnn;
    court c;
    player p1{ vec3{ c_w / 2, c_h, 0 }, 0, vec3{ 0, 255, 0 }, 1 };//player 1
    player p2{ vec3{ c_w / 2, 0,   0 }, 180, vec3{ 255, 0, 0 }, 2 };//player 2
    ball b{ p1.pos + vec3(0, 0, 1) };
    vec2 corner_fps;
};
