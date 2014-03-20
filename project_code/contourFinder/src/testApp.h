#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxOpenCv.h"
#include "ofxKinect.h"
#include "ofxOsc.h"
#include "ofxGui.h"

#define UDP_POINTS_ID 1
#define UDP_MAX_POINT 20
#define UDP_MAX_PACKET 1024
#define HOST "localhost"
#define PORT 1337

template <class T>
void writeRaw(ostringstream& out, T data) {
	out.write((char*) &data, sizeof(data));
}

template <class T>
void readRaw(stringstream& stream, T& data) {
	stream.read((char*) &data, sizeof(T));
}


class Glow : public ofxCv::RectFollower {
protected:
	ofColor color;
	ofVec2f cur, smooth;
	float startedDying;
	ofPolyline all;
public:
	Glow()
		:startedDying(0) {
	}
	void setup(const cv::Rect& track);
	void update(const cv::Rect& track);
	void kill();
	void draw();
    void getPos() { return cur; };
};

class testApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	
    void sendUdp (unsigned short numberBlocks, ofVec3f * pos, ofVec3f * vel, unsigned int * labels);
    
    void keyPressed(int key);
	ofVideoPlayer movie;
	ofxCv::ContourFinder contourFinder;
	ofxCv::RectTrackerFollower<Glow> tracker;
    
    ofxKinect kinect;
    cv::Mat grayImageMat;
    
    ofxCvGrayscaleImage grayImage; // grayscale depth image
	ofxCvGrayscaleImage grayThreshNear; // the near thresholded image
	ofxCvGrayscaleImage grayThreshFar; // the far thresholded image
    
    ofxPanel gui;
    
    ofParameter<int> MinAreaRadius;
    ofParameter<int> MaxAreaRadius;
    ofParameter<int> Treshold;
    ofParameter<int>kinectAngle;
//    ofParameter<ofColor> color;
//    ofParameter<ofVec2f> center;
    ofParameter<int> circleResolution;
    
    // --- OUTPUT ---
    ofxOscSender oscSender;
    bool udpAvailable;
    ofVec3f ** triGrabPoints;
};
