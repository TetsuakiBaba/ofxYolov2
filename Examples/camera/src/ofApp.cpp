#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    // cam
    video.setDeviceID( 0 );
    video.setDesiredFrameRate( 30 );
    video.initGrabber( 640, 480 );

    // yolo
    yolo.setup(ofToDataPath("yolov2-tiny-voc.cfg"),
               ofToDataPath("yolov2-tiny-voc.weights"),
               ofToDataPath("voc.names"));
    
    yolo.setConfidenceThreshold(0.4);
    
  
}

//--------------------------------------------------------------
void ofApp::update(){
    video.update();
    if( video.isFrameNew() ){
        yolo.update(video.getPixels());
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetColor(255);
    video.draw(0,0);
    yolo.draw(0,0,video.getWidth(), video.getHeight());
    ofDrawBitmapString(ofToString(ofGetFrameRate(), 0),20, 20);
}

void ofApp::keyPressed(int key)
{
    if( key == 's' ){
    }
}


