#include "ofApp.h"



//--------------------------------------------------------------
void ofApp::setup(){

    // yolo setting up
    yolo.setup(ofToDataPath("yolov2-tiny.cfg"),
               ofToDataPath("yolov2-tiny.weights"),
               ofToDataPath("coco.names"));
    
    yolo.setConfidenceThreshold(0.4);
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(60);
    ofSetColor(255);

    ofDrawBitmapStringHighlight("Drag and Drop a dataset folder that includes jpg and txt files", 20,20);
   
    // Show YOLO Class Selector
    yolo.drawClassSelector(ofGetWidth()-250,20, 2);

    // Show YOLO annotation
    yolo.drawAnnotation(0,0,
                        yolo.image_annotation.getWidth(),
                        yolo.image_annotation.getHeight());

    // Show YOLO Realtime detection result
    if( flg_show_yolo_detection ){
        yolo.draw(0,0,
                  yolo.image_annotation.getWidth(),
                  yolo.image_annotation.getHeight());
    }
    
    // Show bounding box selecting operation
    ofNoFill();
    ofSetLineWidth(2.0);
    ofDrawRectangle(r);
    

    // show cross 
    ofSetLineWidth(1.0);
    ofSetColor(255,255,255,100);
    ofDrawLine(ofGetMouseX(), 0, ofGetMouseX(), ofGetHeight());
    ofDrawLine(0, ofGetMouseY(), ofGetWidth(), ofGetMouseY());
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if( key == OF_KEY_RIGHT ){
        yolo.setNextAnnotation();
    }
    else if( key == OF_KEY_LEFT ){
        yolo.setPreviousAnnotation();
    }
    else if( key == OF_KEY_CONTROL){
        flg_show_yolo_detection = true;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    if( key == OF_KEY_CONTROL){
        flg_show_yolo_detection = false;
    }
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    ofRectangle r_img;
    r_img.set(0,0, yolo.image_annotation.getWidth(), yolo.image_annotation.getHeight());
    if( r_img.inside(x,y) ){
        r.width = x-r.x;
        r.height = y-r.y;
    }
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    r.set(x,y,0,0);
    
    if( flg_show_yolo_detection == false ){
        for( int i = 0; i < yolo.train.size(); i++ ){
            if( yolo.train.at(i).getScaledBB(yolo.image_annotation.getWidth(), yolo.image_annotation.getHeight()).inside(x,y)){
                yolo.train.erase(yolo.train.begin()+i);
                yolo.saveAnnotation();
            }
        }
    }
    else{
        for( int i = 0; i < yolo.object.size(); i++ ){
            if( yolo.object.at(i).getScaledBB(yolo.image_annotation.getWidth(),
                                              yolo.image_annotation.getHeight()).inside(x,y) ){
                yolo.train.push_back(TrainObject(yolo.object.at(i).class_id,
                                                 yolo.object.at(i).name,
                                                 yolo.object.at(i).r));
                yolo.saveAnnotation();

            }
        }
    }
        
    
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    r.width = x-r.x;
    r.height = y-r.y;
    r.standardize();

    ofRectangle r_img;
    r_img.set(0,0, yolo.image_annotation.getWidth(), yolo.image_annotation.getHeight());
    if( r.getArea() > 10 &&  r_img.inside(x,y) ){
        r.x = r.x/yolo.image_annotation.getWidth();
        r.y = r.y/yolo.image_annotation.getHeight();
        r.width = r.width/yolo.image_annotation.getWidth();
        r.height = r.height/yolo.image_annotation.getHeight();
        
        yolo.addTrainObject(r);
        yolo.saveAnnotation();
    }
    r.set(0,0,0,0);
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
    ofDirectory dir;
    dir.open(dragInfo.files[0]);
    if( dir.isDirectory() ){
        dir.close();
        yolo.loadAnnotationDir(dragInfo.files[0]);
    }
    else{
        ofSystemAlertDialog("Error: Please drop a directory.");
    }
    dir.close();
}
