#pragma once

#include "ofMain.h"
#include "ofxYolo.h"

class ofApp : public ofBaseApp{
public:
    void setup();
    void update();
    void draw();
    ofVideoGrabber video;
    ofxYolo yolo;
    
    void keyPressed(int key);
};
