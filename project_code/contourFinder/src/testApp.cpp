#include "testApp.h"

using namespace ofxCv;
using namespace cv;

const float dyingTime = 1;

void Glow::setup(const cv::Rect& track) {
	color.setHsb(ofRandom(0, 255), 255, 255);
	cur = toOf(track).getCenter();
	smooth = cur;
}

void Glow::update(const cv::Rect& track) {
	cur = toOf(track).getCenter();
	smooth.interpolate(cur, .5);
	all.addVertex(smooth);
}

void Glow::kill() {
	float curTime = ofGetElapsedTimef();
	if(startedDying == 0) {
		startedDying = curTime;
	} else if(curTime - startedDying > dyingTime) {
		dead = true;
	}
}

void Glow::draw() {
	ofPushStyle();
	float size = 26;
	ofSetColor(255);
	if(startedDying) {
		ofSetColor(ofColor::red);
		size = ofMap(ofGetElapsedTimef() - startedDying, 0, dyingTime, size, 0, true);
	}
	ofNoFill();
	ofCircle(cur, size);
	ofSetColor(color);
	all.draw();
	ofSetColor(255);
	ofDrawBitmapString(ofToString(label), cur);
	ofPopStyle();
}

void testApp::setup() {
	ofSetVerticalSync(true);
	ofBackground(0);
	ofSetFrameRate(30);
    
    kinect.setRegistration(true);
    
	//kinect.init(true); // shows infrared instead of RGB video image
	kinect.init(false, false); // disable video image (faster fps)
	
	kinect.open();
    kinect.setLed(ofxKinect::LED_OFF);
    gui.setup("panel");
    gui.add(MinAreaRadius.set( "minArea", 10,10, 200 ));
    gui.add(MaxAreaRadius.set( "maxArea", 50,10, 200 ));
    gui.add(Treshold.set( "Treshold", 10,10, 200 ));
    gui.add(kinectAngle.set("ANGLE", 0, -30, 30));
    gui.setPosition(ofPoint(600, 10));
    
    contourFinder.setMinAreaRadius(MinAreaRadius);
	contourFinder.setMaxAreaRadius(MaxAreaRadius);
	contourFinder.setThreshold(Treshold); // min Z value
//    contourFinder.setAutoThreshold(true);
//        contourFinder.set
	// wait for half a frame before forgetting something
	tracker.setPersistence(15);
	// an object can move up to 50 pixels per frame
	tracker.setMaximumDistance(70);

    // --- OUTPUT ---
    oscSender.setup(HOST, PORT);
}

void testApp::update() {
//    kinect.setCameraTiltAngle(kinectAngle);
    kinect.update();
	
	// there is a new frame and we are connected
	if(kinect.isFrameNew()) {
        // load grayscale depth image from the kinect source
        grayImage.setFromPixels(kinect.getDepthPixels(), kinect.width, kinect.height);
		grayImageMat = Mat(kinect.height, kinect.width, CV_8UC1, kinect.getDepthPixels(), 0);

//        Mat farThreshMat = grayImageMat;
//        Mat  nearThreshMat = grayImageMat;
//        threshold(nearThreshMat,100, true);
//        threshold(farThreshMat,10);
//         bitwise_and(nearThreshMat, farThreshMat, grayImageMat, NULL);
		// we do two thresholds - one for the far plane and one for the near plane
		// we then do a cvAnd to get the pixels which are a union of the two thresholds
//			grayThreshNear = grayImage;
//			grayThreshFar = grayImage;
//			grayThreshNear.threshold(1000, true);
//			grayThreshFar.threshold(10);
//			cvAnd(grayThreshNear.getCvImage(), grayThreshFar.getCvImage(), grayImage.getCvImage(), NULL);
//        grayImage.flagImageChanged();
//        grayImageMat = Mat(kinect.height, kinect.width, CV_8UC1, grayImage.getPixels(), 0);
        
        blur(grayImageMat, 15);
        contourFinder.setMinAreaRadius(MinAreaRadius);
        contourFinder.setMaxAreaRadius(MaxAreaRadius);
        contourFinder.setThreshold(Treshold); // min Z valu
		contourFinder.findContours(grayImageMat);

		tracker.track(contourFinder.getBoundingRects());
//        vector<Glow>& followers = tracker.getFollowers();

        if (contourFinder.size() > 0) {

            ofVec3f * pos = new ofVec3f [contourFinder.size()];
            ofVec3f * vel = new ofVec3f [contourFinder.size()];
            unsigned int * labels = new unsigned int [contourFinder.size()];
            for(int i = 0; i < contourFinder.size(); i++) {
//                ofPoint center = toOf(contourFinder.getCenter(i));
                ofVec2f center = toOf(contourFinder.getCenter(i));
                center.interpolate(center, .5);
                labels[i] = contourFinder.getLabel(i);
                vel[i] = toOf(contourFinder.getVelocity(i));
                pos[i] = ofVec3f(center.x, center.y, grayImage.getPixels()[int(center.y*kinect.width+center.x)]);
                
                ofxOscMessage m;
                m.setAddress("/kinect/point");
                m.addIntArg(contourFinder.getLabel(i));
                m.addIntArg(pos[i].x);
                m.addIntArg(pos[i].y);
                m.addIntArg(pos[i].z);
                m.addIntArg(vel[i].x);
                m.addIntArg(vel[i].y);
                oscSender.sendMessage(m);
//                printf("%i", m.getArgAsInt32(1));
                //                sendUdp(contourFinder.size(), pos, vel, labels);
            }
            
//            sendUdp(contourFinder.size(), pos, vel, labels);
            
            delete [] pos;
            delete [] vel;
            delete [] labels;
        } else {
            ofxOscMessage m;
            m.setAddress("/kinect/clear");
            m.addIntArg(1);
            oscSender.sendMessage(m);
        }
        
//        for(int i = 0; i < followers.size(); i++) {
//            followers[i].getPos();
//        }
    }
}

void testApp::draw() {
	ofSetColor(255);
//	movie.draw(0, 0);
//    kinect.drawDepth(0, 0, kinect.width, kinect.height);
    drawMat(grayImageMat, 0,0,kinect.width,kinect.height);
	contourFinder.draw();
    
    for(int i = 0; i < contourFinder.size(); i++) {
        ofPoint center = toOf(contourFinder.getCenter(i));
        ofPushMatrix();
        ofTranslate(center.x, center.y);
        int label = contourFinder.getLabel(i);
        ofVec2f velocity = toOf(contourFinder.getVelocity(i));
        ofVec3f obj = ofVec3f(center.x, center.y, grayImage.getPixels()[int(center.y*kinect.width+center.x)]);
//        ofVec3f obj = toOf(contourFinder.getCenter(i));
        ofDrawBitmapString(ofToString(obj), 0,0);
        ofScale(5, 5);
        ofLine(0, 0, velocity.x, velocity.y);
        ofPopMatrix();
    }

    
    gui.draw();
//    vector<Glow>& followers = tracker.getFollowers();
//	for(int i = 0; i < followers.size(); i++) {
//		followers[i].draw();
//	}

    ofDrawBitmapString(ofToString(ofGetFrameRate()), 20, ofGetHeight()-30);
    ofDrawBitmapString("SIZE"+ofToString(contourFinder.size()), 100, ofGetHeight()-30);
}

void testApp::sendUdp (unsigned short numberBlocks, ofVec3f * pos, ofVec3f * vel, unsigned int * labels) {

    ostringstream msg(ostringstream::binary);

    unsigned short blockID;
    unsigned int blockSize;

    writeRaw(msg, UDP_POINTS_ID); //updPacket.devices
    writeRaw(msg, numberBlocks); //
//    writeRaw(msg, sourceCount); //

    for (int i=0;i<numberBlocks;i++) {
        writeRaw(msg, labels[i]);
        writeRaw(msg, pos[i]); 
        writeRaw(msg, vel[i]);
    }
    
    writeRaw(msg, '\0'); // empty null-terminated string
    string msgString = msg.str();
//    printf("%c \n", msgString.c_str());

//    udpConnection.Send(msgString.c_str(), msgString.length());
}

void testApp::keyPressed(int key){
    
    if (key == 's') {
        
        gui.saveToFile("GUI.xml");
    }
    
    if(key == 'l') {
        gui.loadFromFile("GUI.xml");
    }
}

