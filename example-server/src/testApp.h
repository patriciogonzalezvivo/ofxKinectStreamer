#pragma once

#include "ofMain.h"

#include "ofxNetwork.h"
#include "ofxGui.h"
#include "ofxOsc.h"

#define FAKEKINECT

#include "ofxKinectStreamer.h"

class testApp : public ofBaseApp{
public:
    void setup();
    void update();
    void draw();

    void keyPressed  (int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
	
	void exit();
	
private:
    ofxKinectServer *frameSender;     //  needs to be a pointer as FrameSender 
                                        //  uses POCO classes with private constructors, 
                                        //  and then can't be copied
    
    ofxOscReceiver  oscReceiver;
    
#ifdef FAKEKINECT
    ofVideoGrabber  grabber;
#else
    ofxKinect       kinect;
#endif
    
    ofxPanel        gui;
    ofxIntSlider    minDist;
    ofxIntSlider    maxDist;
    
    ofPixels    pixels;
    ofShader    shader;
    ofFbo       fbo;
    
    int width, height;
};
