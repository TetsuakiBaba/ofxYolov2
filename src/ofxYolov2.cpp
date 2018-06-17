#include "ofxYolov2.h"

Object::Object(int _class_id, string _name, float _p, float _x, float _y, float _w, float _h)
{
    class_id = _class_id;
    name = _name;
    p = _p;
    r.set(_x, _y, _w, _h);
}

Object::~Object()
{
    
}


ofxYolov2::ofxYolov2()
{
    
}

ofRectangle Object::getScaledBB(float _w, float _h)
{
    ofRectangle r_return;
    r_return.set(r.x*_w,
                 r.y*_h,
                 r.width * _w,
                 r.height * _h);
    return r_return;
}

ofxYolov2::~ofxYolov2()
{
    
}

void ofxYolov2::draw(float _x, float _y, float _w, float _h)
{
    ofNoFill();
    ofSetLineWidth(3);
    ofSetColor(255,255,255, 100);
    for( int i = 0; i < object.size(); i++ ){
        ofRectangle r_scaled = object.at(i).getScaledBB(_w, _h);
        ofDrawRectangle(r_scaled);
        ofDrawBitmapStringHighlight("["+ofToString(i)+"]: "+object.at(i).name + ": " + ofToString(object.at(i).p),
                                    r_scaled.x,r_scaled.y-10);
    }
    ofSetColor(255,255,255, 255);
}

cv::Mat ofxYolov2::toCV(ofPixels &pix)
{
    return cv::Mat(pix.getHeight(), pix.getWidth(), CV_MAKETYPE(CV_8U, pix.getNumChannels()), pix.getData(), 0);
}

void ofxYolov2::update(ofPixels &op)
{
    object.clear();
    
    cv::Mat frame = toCV(op);
    
    if (frame.channels() == 4)
        cvtColor(frame, frame, COLOR_BGRA2BGR);
    
    //! [Resizing without keeping aspect ratio]
    Mat resized;
    resize(frame, resized, cvSize(network_width, network_height));
    
    //! [Prepare blob]
    Mat inputBlob = blobFromImage(resized, 1 / 255.F); //Convert Mat to batch of images
    
    //! [Set input blob]
    net.setInput(inputBlob, "data");                   //set the network input
    
    //! [Make forward pass]
    Mat detectionMat = net.forward("detection_out");   //compute output
    
    
    for (int i = 0; i < detectionMat.rows; i++)
    {
        
        const int probability_index = 5; // do not change.
        const int probability_size = detectionMat.cols - probability_index;
        float *prob_array_ptr = &detectionMat.at<float>(i, probability_index);
        
        size_t objectClass = max_element(prob_array_ptr, prob_array_ptr + probability_size) - prob_array_ptr;
        float confidence = detectionMat.at<float>(i, (int)objectClass + probability_index);
        
        if (confidence > confidenceThreshold)
        {
            float x = detectionMat.at<float>(i, 0);
            float y = detectionMat.at<float>(i, 1);
            float width = detectionMat.at<float>(i, 2);
            float height = detectionMat.at<float>(i, 3);
            int xLeftBottom = static_cast<int>((x - width / 2) * frame.cols);
            int yLeftBottom = static_cast<int>((y - height / 2) * frame.rows);
            int xRightTop = static_cast<int>((x + width / 2) * frame.cols);
            int yRightTop = static_cast<int>((y + height / 2) * frame.rows);
            

            if (objectClass < classNamesVec.size())
            {
                String label = String(classNamesVec[objectClass]);
                ofRectangle r(x-width/2.0,y-height/2.0,width,height);
                object.push_back(Object(objectClass, label, confidence, r.x,r.y, r.width, r.height));
                
            }
            else
            {
                cout << "Class: " << objectClass << endl;
                cout << "Confidence: " << confidence << endl;
                cout << " " << xLeftBottom
                << " " << yLeftBottom
                << " " << xRightTop
                << " " << yRightTop << endl;
            }
        }
    }
}

void ofxYolov2::setup(string _path_to_cfg, string _path_to_weights, string _path_to_names)
{
    String modelConfiguration = ofToDataPath(_path_to_cfg);
    String modelBinary = ofToDataPath(_path_to_weights);
    
    //! [Initialize network]
    net = readNetFromDarknet(modelConfiguration, modelBinary);

    if (net.empty())
    {
        cout << "Can't load network by using the following files: " << endl;
        cout << "cfg-file:     " << modelConfiguration << endl;
        cout << "weights-file: " << modelBinary << endl;
        cout << "Models can be downloaded here:" << endl;
        cout << "https://pjreddie.com/darknet/yolo/" << endl;
    }
    
    // objectClassName
    ifstream classNamesFile(ofToDataPath(_path_to_names));
    if (classNamesFile.is_open())
    {
        string className = "";
        while (std::getline(classNamesFile, className)){
            classNamesVec.push_back(className);
        }
        
        for( auto itr : classNamesVec )
        {
            string cName = itr;
            cout << "classNames :" << cName << endl;
        }
    }
    
    confidenceThreshold = 0.6;
}

void ofxYolov2::setConfidenceThreshold(float _threshold)
{
    if( 0.0 <= _threshold && _threshold <= 1.0){
        confidenceThreshold = _threshold;
    }
}
