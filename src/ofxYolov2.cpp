#include "ofxYolov2.h"

std::string Replace( std::string String1, std::string String2, std::string String3 )
{
    std::string::size_type  Pos( String1.find( String2 ) );
    
    while( Pos != std::string::npos )
    {
        String1.replace( Pos, String2.length(), String3 );
        Pos = String1.find( String2, Pos + String3.length() );
    }
    
    return String1;
}

bool doesIncludeExtensions(string _filename, vector<string>_extensions)
{
    string extname;
    
    int ext_i = _filename.find_last_of(".");
    if( ext_i > 0 ){
        extname = _filename.substr(ext_i,_filename.size()-ext_i);
    }
    else{
        return false;
    }
    
    for( int i = 0; i < _extensions.size(); i++ ){
        if( _extensions[i] == extname ){
            return true;
        }
    }
    return false;
}


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

ofRectangle TrainObject::getScaledBB(float _w, float _h)
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
    for( int i = 0; i < object.size(); i++ ){
        ofNoFill();
        ofSetLineWidth(3);
        ofSetColor(detection_color.at(object[i].class_id));
        ofRectangle r_scaled = object.at(i).getScaledBB(_w, _h);
        ofDrawRectangle(r_scaled);
        
        ofFill();
        ofDrawRectangle(r_scaled.x, r_scaled.y-18,r_scaled.width,18);
        ofSetColor(ofColor::white);
        font_info.drawString("["+ofToString(object.at(i).class_id)+"]: "+object.at(i).name + ": " + ofToString(object.at(i).p),
                                    r_scaled.x,r_scaled.y);
    }
}

void ofxYolov2::drawAnnotation(float _x, float _y, float _w, float _h)
{
    ofSetColor(255);
    if( image_annotation.isAllocated() ){
        image_annotation.draw(0,0, _w, _h);
    }

    for( int i = 0; i < train.size(); i++ ){
        ofNoFill();
        ofSetLineWidth(3);
        ofSetColor(detection_color.at(train[i].id),150);
        ofRectangle r_scaled = train.at(i).getScaledBB(_w, _h);

        ofDrawRectangle(r_scaled);
        
        ofFill();
        ofDrawRectangle(r_scaled.x, r_scaled.y-18,r_scaled.width,18);
        ofSetColor(ofColor::white);
        font_info.drawString("["+ofToString(train.at(i).id)+"]: "+train.at(i).name,                          r_scaled.x,r_scaled.y);

    }
    

    for( int i = 0; i < train.size(); i++ ){
        ofSetColor(detection_color[train[i].id],150);
        if( train[i].getScaledBB(image_annotation.getWidth(), image_annotation.getHeight()). inside(ofGetMouseX(), ofGetMouseY()) ){
            ofFill();
            ofDrawRectangle(train[i].getScaledBB(image_annotation.getWidth(), image_annotation.getHeight()));
        }
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
    String modelConfiguration = _path_to_cfg;
    String modelBinary = _path_to_weights;
    
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
    cout << _path_to_names << endl;
    ifstream classNamesFile(_path_to_names);
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
    
    // set default Detection Color
    for( int i = 0; i < classNamesVec.size(); i++ ){
        detection_color.push_back(ofColor(100*((i+1)%2)+100,
                                          50*((i+2)%3)+100,
                                          25*((i+3)%4)+100,
                                          255));
    }
    
    // for gocen detection color
    /*
    {
        ofColor c[] =
        {
            ofColor(255, 255, 255),
            ofColor(111, 12, 188),
            ofColor(25, 71, 175),
            ofColor(188, 12, 180),
            ofColor(200, 255, 255),
            ofColor(255, 230, 51),
            ofColor(34, 181, 115),
            ofColor(98, 160, 0),
            ofColor(64, 109, 5),
            ofColor(255, 137, 57),
            ofColor(255, 85, 85),
            ofColor(255, 102, 186),
            ofColor(140, 98, 57),
            ofColor(238, 118, 255),
            ofColor(117, 158, 255)
        };
        
        for( int i = 0; i < classNamesVec.size(); i++){
            detection_color.push_back(c[i]);
        }
    }
     */
    
    font_info.load(ofToDataPath("DIN Alternate Bold.ttf"),12);
    confidenceThreshold = 0.6;
    class_id_selected = 0; // default 
}

void ofxYolov2::setConfidenceThreshold(float _threshold)
{
    if( 0.0 <= _threshold && _threshold <= 1.0){
        confidenceThreshold = _threshold;
    }
}

void ofxYolov2::loadBoundingBoxFile(string _path_to_file)
{
    train.clear();
    vector<string>str_bb;
    
    if( !ofFile::doesFileExist(_path_to_file) ){
        cout << "no such a file" << endl;
        ofSystem("touch "+_path_to_file);
    }
    ofBuffer ofbuf = ofBufferFromFile(_path_to_file);

    ofbuf.getLines();
    for( auto line: ofbuf.getLines() ){
        str_bb.push_back(line);
    }
    
    for( int j = 0; j < str_bb.size(); j++ ){
        auto string = str_bb[j];
        auto separator = std::string(" ");
        auto separator_length = separator.length();
        auto list = std::vector<std::string>();
        
        if (separator_length == 0) {
            list.push_back(string);
        }
        else{
            auto offset = std::string::size_type(0);
            while (1) {
                auto pos = string.find(separator, offset);
                if (pos == std::string::npos) {
                    list.push_back(string.substr(offset));
                    break;
                }
                list.push_back(string.substr(offset, pos - offset));
                offset = pos + separator_length;
            }
        }
        
        
        if( list.size() == 5 ){
            float x,y,w,h;
            x = ofToFloat(list[1]);
            y = ofToFloat(list[2]);
            w = ofToFloat(list[3]);
            h = ofToFloat(list[4]);
            x = x -w/2.0;
            y = y -h/2.0;
            ofRectangle r(x,y,w,h);
            TrainObject t(ofToInt(list[0]), classNamesVec.at(ofToInt(list[0])), r);
            train.push_back(t);
        }
    }
}

void ofxYolov2::saveAnnotation()
{
    saveBoundingBoxToFile(filename_txt);
}
void ofxYolov2::loadAnnotationDir(string _path_to_file)
{
    train.clear();
    filepath = _path_to_file;
    pos_annotation_file = 0;
    dir_annotation.open(filepath);
    dir_annotation.allowExt("jpg");
    dir_annotation = dir_annotation.getSorted();

    filename_jpg = dir_annotation.getPath(pos_annotation_file);
    filename_txt = Replace(dir_annotation.getPath(pos_annotation_file),".jpg", ".txt");

    // Load first jpg image
    loadAnnotationImage(filename_jpg);
    loadBoundingBoxFile(filename_txt);
}
void ofxYolov2::loadAnnotationImage(string _path_to_file)
{
    image_annotation.load(_path_to_file);
    update(image_annotation.getPixels());
}

void ofxYolov2::saveBoundingBoxToFile(string _path_to_file)
{
    ofFile file;
    if( file.open(_path_to_file, ofFile::WriteOnly) ){
        for( int i = 0; i < train.size(); i++){
            train[i].r.standardize();
            file <<
            train[i].id << " " <<
            train[i].r.x + train[i].r.getWidth()/2.0 << " " <<
            train[i].r.y + train[i].r.getHeight()/2.0 << " " <<
            train[i].r.getWidth() << " " <<
            train[i].r.getHeight() << endl;
        }
        file.close();
        train.clear();
        loadBoundingBoxFile(_path_to_file);
    }
    else{
        cout << "failed" << endl;
    }
    
}

void ofxYolov2::setNextAnnotation()
{
    pos_annotation_file++;
    if( pos_annotation_file > dir_annotation.size()-1 ){
        pos_annotation_file = dir_annotation.size()-1;
    }
    filename_jpg = dir_annotation.getPath(pos_annotation_file);
    filename_txt = Replace(dir_annotation.getPath(pos_annotation_file),".jpg", ".txt");
    loadAnnotationImage(filename_jpg);
    loadBoundingBoxFile(filename_txt);
}

void ofxYolov2::setPreviousAnnotation()
{
    pos_annotation_file--;
    if( pos_annotation_file < 0 ){
        pos_annotation_file=0;
    }
    filename_jpg = dir_annotation.getPath(pos_annotation_file);
    filename_txt = Replace(dir_annotation.getPath(pos_annotation_file),".jpg", ".txt");
    loadAnnotationImage(filename_jpg);
    loadBoundingBoxFile(filename_txt);
}

void ofxYolov2::drawClassSelector(float _x, float _y, int _row)
{
    // Show Class Selector
    int row = _row;
    float x,y;

    ofRectangle r_area(_x,
                       _y,
                       100*row,
                       14*classNamesVec.size()/row);

    ofNoFill();
    ofDrawRectangle(r_area);
    font_info.drawString("Select a class name",r_area.x,r_area.y);
    for( int i = 0; i < classNamesVec.size(); i++ ){
        x = _x+100*(i%row);
        y = _y+14+14*(i/row);
        ofNoFill();
        ofSetColor(detection_color.at(i));
        ofRectangle r_name;
        r_name.set(font_info.getStringBoundingBox(classNamesVec.at(i),
                                                  x,y));

        if( r_name.inside(ofGetMouseX(),ofGetMouseY()) ){
            if( ofGetMousePressed() ){
                class_id_selected = i;
            }
            else{
                ofDrawRectangle(r_name);
            }
        }
        
        if( class_id_selected == i ){
            ofFill();
            ofDrawRectangle(r_name);
            
            ofSetColor(detection_color.at(i).getInverted());
            font_info.drawString(classNamesVec.at(i),x,y);
                                 
        }
        else{
            font_info.drawString(classNamesVec.at(i),x,y);
        }
    }
    
}

void ofxYolov2::addTrainObject(ofRectangle _r)
{
    train.push_back(TrainObject(class_id_selected, classNamesVec.at(class_id_selected), _r));
}

