#include "ofxYolov2.h"

Object::Object(string _name, float _p, float _x, float _y, float _w, float _h)
{
    name = _name;
    p = _p;
    r.set(_x, _y, _w, _h);
}

ofxYolov2::ofxYolov2()
{
    
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
        float x = object.at(i).r.x * _w - (object.at(i).r.width*_w)/2.0;
        float y = object.at(i).r.y * _h - (object.at(i).r.height*_h)/2.0;
        float w = object.at(i).r.width * _w;
        float h = object.at(i).r.height * _h;
        ofDrawRectangle(x, y, w, h);

        ofDrawBitmapStringHighlight(object.at(i).name + ": " + ofToString(object.at(i).p),
                                    x,y);
    }
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
    //! [Resizing without keeping aspect ratio]
    
    //! [Prepare blob]
    Mat inputBlob = blobFromImage(resized, 1 / 255.F); //Convert Mat to batch of images
    //! [Prepare blob]
    
    //! [Set input blob]
    net.setInput(inputBlob, "data");                   //set the network input
    //! [Set input blob]
    
    //! [Make forward pass]
    Mat detectionMat = net.forward("detection_out");   //compute output
    //! [Make forward pass]
    
    /*
     vector<double> layersTimings;
     double freq = getTickFrequency() / 1000;
     double time = net.getPerfProfile(layersTimings) / freq;
     ostringstream ss;
     ss << "FPS: " << 1000/time << " ; time: " << time << " ms";
     putText(frame, ss.str(), cvPoint(20,20), 0, 0.5, Scalar(0,0,255));
     */
    
    for (int i = 0; i < detectionMat.rows; i++)
    {
        
        const int probability_index = 5;
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
            
            /*
             rectangle(frame, cvRect(xLeftBottom, yLeftBottom,xRightTop - xLeftBottom,yRightTop - yLeftBottom), Scalar(0, 255, 0));
             */
            
            
            if (objectClass < classNamesVec.size())
            {
                /*
                 ss.str("");
                 ss << confidence;
                 String conf(ss.str());
                 */
                String label = String(classNamesVec[objectClass]);
                //                int baseLine = 0;
                
                /*
                 CvSize labelSize = getTextSize(label, FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseLine);
                 rectangle(frame, cvRect(xLeftBottom, yLeftBottom ,labelSize.width, labelSize.height + baseLine),Scalar(255, 255, 255), CV_FILLED);
                 putText(frame, label, cvPoint(xLeftBottom, yLeftBottom + labelSize.height),
                 FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0,0,0));
                 */
                ofRectangle r(x,y,width,height);
                object.push_back(Object(label, confidence, x,y, width, height));
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
    String modelConfiguration = ofToDataPath(_path_to_cfg); // YOLOの.cfgファイルへのパスを記載
    String modelBinary = ofToDataPath(_path_to_weights); // YOLOの.weightsファイルへのパスを記載
    

    //! [Initialize network]
    net = readNetFromDarknet(modelConfiguration, modelBinary);
    //! [Initialize network]
    if (net.empty())
    {
        cout << "Can't load network by using the following files: " << endl;
        cout << "cfg-file:     " << modelConfiguration << endl;
        cout << "weights-file: " << modelBinary << endl;
        cout << "Models can be downloaded here:" << endl;
        cout << "https://pjreddie.com/darknet/yolo/" << endl;
    }
    
    // objectClassName
    ifstream classNamesFile(ofToDataPath("coco.names")); // YOLOの.ClassNameファイルへのパスを記載
    if (classNamesFile.is_open())
    {
        string className = "";
        while (std::getline(classNamesFile, className))
            classNamesVec.push_back(className);
        
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
