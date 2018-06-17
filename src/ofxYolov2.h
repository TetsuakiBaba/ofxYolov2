#pragma once

#include "ofMain.h"
#include <opencv2/dnn.hpp>
#include <opencv2/dnn/shape_utils.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <cstdlib>

using namespace std;
using namespace cv;
using namespace cv::dnn;

class Object{
public:
    Object(int _class_id, string _name, float _p, float _x, float _y, float _w, float _h);
    ~Object();
    ofRectangle r;
    ofRectangle getScaledBB(float _w, float _h);
    string name;
    float p;
    int class_id;
};

class ofxYolov2{
public:
    ofxYolov2();
    ~ofxYolov2();
    void setup(string _path_to_cfg, string _path_to_weights, string _path_to_names);
    void draw(float _x, float _y, float _w, float _h);
    void update(ofPixels &op);
    void setConfidenceThreshold(float _threshold);

    vector<Object> object;
    cv::Mat toCV(ofPixels &pix);
    dnn::Net net;
    int network_width = 416;
    int network_height = 416;
    vector<string> classNamesVec;
    float confidenceThreshold;
    ofTrueTypeFont font_info;
};
