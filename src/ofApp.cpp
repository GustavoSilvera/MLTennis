#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    std::ofstream ofs;
    ofs.open("DATA.txt", std::ofstream::out | std::ofstream::trunc);
    ofs.close();
    srand(time(NULL));//seeds random number generator
    w_x = ofGetWidth() / 2 - ppm * c_w / 2;
    w_y = ofGetHeight() / 2 - ppm * c_h / 2;
    //mFont = Font("Arial", ppm);//fixed custom font
    //mTextureFont = gl::TextureFont::create(mFont);
    renderFont();
    corner_fps.x = ofGetWidth() - 8 * ppm;
    corner_fps.y = 0 + 2 * ppm;
    drawScore(&b);//updates score when ball dies
}

//--------------------------------------------------------------
void ofApp::update(){
    const int dt = 60;
    for (int i = 0; i < mult; i++) {
        b.update(&c, dt);
        p1.update(&b, &c, &p2, &cnn, dt);
        p2.update(&b, &c, &p1, &cnn, dt);
    }
}
void ofApp::drawScore(ball* b){
    float hor_shift = 1;//meters from edge of court
    float ver_shift = 2.5;//meters from edge of court
    ofSetColor(255, 255, 255);
    //player 2
    string titles[3] = { "Set: ", "Game: ", "Point: " };
    int values_p2[3] = { b->set_p2, b->game_p2, b->point_p2 };
    int values_p1[3] = { b->set_p1, b->game_p1, b->point_p1 };
    for (int i = 0; i < 3; i++) {//player 2
        vec2 pos = ppm * vec2{ c_w + hor_shift, i * ver_shift };
        drawFontText(titles[i], pos);
        pos.x += 5 * ppm;//space btwn title & value
        drawFontText(values_p2[i], pos);
    }
    for (int i = 0; i < 3; i++) {//player 1
        vec2 pos = ppm * vec2{ c_w + hor_shift, c_h - (2 - i) * ver_shift };
        drawFontText(titles[i], pos);
        pos.x += 5 * ppm;//space btwn title & value
        drawFontText(values_p1[i], pos);
    }
}
//--------------------------------------------------------------
void ofApp::draw(){
    glPushMatrix();
    ofTranslate(w_x, w_y);
    // clear out the window with black
    /*if (b.pos.x > c_w || b.pos.x < 0 || b.pos.y > c_h || b.pos.y < 0 || b.died) {//if either the ball is out of the court or the player is
        }*///OPTIMIZATION TIIIP
    ofSetBackgroundColor(0, 0, 0); //ONLY CLEAR WHENN NECESSARY
    drawScore(&b);//updates score when ball dies
    c.draw();
    p1.draw();
    p2.draw();
    b.draw();
    glPopMatrix();
    ofSetColor(0, 255, 0);
    //draw black rectangle behind FPS to prevent overdraw
    drawFontText(ofGetFrameRate(), corner_fps);
    ofSetColor(255, 255, 255);
    drawFontText(mult, vec2(corner_fps.x, corner_fps.y + 2 * ppm));
    drawFontText(b.num_hits, vec2(corner_fps.x, corner_fps.y + 3 * ppm));
    drawFontText(b.num_good_hits, vec2(corner_fps.x, corner_fps.y + 4 * ppm));
    drawFontText(b.num_good_hits / (float)b.num_hits, vec2(corner_fps.x, corner_fps.y + 5 * ppm));
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == OF_KEY_UP) {//begins the training
        mult *= 10;
        c.updateFrames();
        b.updateFrames(mult);
    }
    if (key == OF_KEY_DOWN && mult >= 10) {
        mult /= 10;
        c.updateFrames();
        b.updateFrames(mult);
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}
