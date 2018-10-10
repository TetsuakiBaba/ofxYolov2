#pragma once

#include "ofMain.h"
#include "ofxYolov2.h"

class ofApp : public ofBaseApp{
public:
    void setup();
    void update();
    void draw();
    ofVideoGrabber video;
    ofxYolov2 yolo;
    
    void keyPressed(int key);
    ofImage img;
};
